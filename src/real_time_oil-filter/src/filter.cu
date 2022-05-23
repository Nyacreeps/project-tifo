#include "filter.cuh"

[[gnu::noinline]]
void _abortError(const char *msg, const char *fname, int line) {
    cudaError_t err = cudaGetLastError();
    spdlog::error("{} ({}, line: {})", msg, fname, line);
    spdlog::error("Error {}: {}", cudaGetErrorName(err), cudaGetErrorString(err));
    std::exit(1);
}

#define abortError(msg) _abortError(msg, __FUNCTION__, __LINE__)

template <int radius>
constexpr bool *circle_mask() {
    int size = 2 * radius + 1;
    auto result = (bool *)malloc(sizeof(bool) * size * size);
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            result[i * size + j] = std::pow(i - radius, 2) + std::pow(j - radius, 2) <= std::pow(radius, 2);
    return result;
}

unsigned char *flatten(unsigned char **buffer_, const int width, const int height) {
    auto result = (unsigned char *)malloc(sizeof(unsigned char) * width * height);
#pragma omp parallel for schedule(dynamic) shared(buffer_, result, width, height) default(none) collapse(2) nowait
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            result[i * width + j] = buffer_[i][j];
    return result;
}

unsigned char **unflatten(unsigned char *buffer_, const int width, const int height) {
    auto result = (unsigned char **)malloc(sizeof(unsigned char *) * height);
    for (int i = 0; i < height; result[i++] = (unsigned char *)malloc(sizeof(unsigned char) * width));
#pragma omp parallel for schedule(dynamic) shared(buffer_, result, width, height) default(none) collapse(2) nowait
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            result[i][j] = buffer_[i * width + j];
    return result;
}

__global__ void compute_intensities(unsigned char *buffer, size_t bufferPitch, size_t width, size_t height,
                        unsigned char *res, size_t resPitch)
{
    const int x = blockDim.x * blockIdx.x + threadIdx.x;
    const int y = blockDim.y * blockIdx.y + threadIdx.y;

    if (x >= width || y >= height)
        return;

    const auto *buffer_line = buffer + y * bufferPitch;
    auto *res_line = res + y * resPitch;
    const auto r = buffer_line[x * 3];
    const auto g = buffer_line[x * 3 + 1];
    const auto b = buffer_line[x * 3 + 2];

    int intensity = (int)((double)(r + g + b) / 3 * INTENSITY / 255);
    res_line[x] = intensity < 0 ? 0 : intensity > 255 ? 255 : intensity;
}

template <int radius>
__global__ void get_Imax(unsigned char *I, size_t I_Pitch, size_t width, size_t height,
                             const bool *mask, size_t maskPitch, unsigned char *I_max, size_t I_maxPitch)
{
    const int x = blockDim.x * blockIdx.x + threadIdx.x;
    const int y = blockDim.y * blockIdx.y + threadIdx.y;

    if (x >= width || y >= height)
        return;

    unsigned char intensityBins[256];
    for (int i = 0; i < 256; intensityBins[i++] = 0);

    unsigned char max = 0;
    unsigned char imax = 0;
    for (int i = 0; i < 2 * radius + 1; ++i) {
        if (y + i - radius >= 0 && y + i - radius < height) {
            const auto *mask_line = (bool *) ((char *) mask + i * maskPitch);
            for (int j = 0; j < 2 * radius + 1; ++j) {
                const auto idxh = y + i - radius;
                const auto idxw = x + j - radius;
                if (mask_line[j] && idxw >= 0 && idxw < width) {
                    const auto intensity = (I + idxh * I_Pitch)[idxw];
                    auto tmp = ++intensityBins[intensity];
                    if (tmp > max) {
                        max = tmp;
                        imax = intensity;
                    }
                }
            }
        }
    }
    (I_max + y * I_maxPitch)[x] = imax;
}

template <int radius>
__global__ void apply_filter(unsigned char *buffer, size_t bufferPitch, size_t width, size_t height,
                             unsigned char *res, size_t resPitch, const bool *mask, size_t maskPitch,
                             unsigned char *I, size_t I_Pitch, unsigned char *I_max, size_t I_maxPitch)
{
    const int x = blockDim.x * blockIdx.x + threadIdx.x;
    const int y = blockDim.y * blockIdx.y + threadIdx.y;

    if (x >= width || y >= height)
        return;

    auto *res_line = (unsigned char *) ((char *) res + y * resPitch);
    const auto indmax = (I_max + y * I_maxPitch)[x];

    int R = 0, G = 0, B = 0;
    unsigned char n = 0;
    for (int i = 0; i < 2 * radius + 1; ++i) {
        if (y + i - radius >= 0 && y + i - radius < height) {
            const auto *buffer_line = buffer + (y + i - radius) * bufferPitch;
            const auto *mask_line = (bool *)((char *)mask + i * maskPitch);
            for (int j = 0; j < 2 * radius + 1; ++j) {
                const auto idxh = y + i - radius;
                const auto idxw = x + j - radius;
                if (mask_line[j] && idxw >= 0 && idxw < width) {
                    const auto intensity = (I + idxh * I_Pitch)[idxw];
                    if (intensity == indmax) {
                        const auto color = &(buffer_line[idxw * 3]);
                        R += (int) color[0];
                        G += (int) color[1];
                        B += (int) color[2];
                        ++n;
                    }
                }
            }
        }
    }

    res_line[x * 3] = (int)round((double)R / (double)n);
    res_line[x * 3 + 1] = (int)round((double)G / (double)n);
    res_line[x * 3 + 2] = (int)round((double)B / (double)n);
}


unsigned char **oil_filter(unsigned char **buffer_, const int width, const int height)
{
    int bsize = 32;
    int w = std::ceil((float) width / (float)bsize);
    int h = std::ceil((float) height / (float)bsize);

    spdlog::debug("running kernel of size ({},{})", w, h);

    dim3 dimBlock(bsize, bsize);
    dim3 dimGrid(w, h);

    // Device image buffer
    unsigned char *devBuffer;
    size_t BufferPitch;
    if (cudaMallocPitch(&devBuffer, &BufferPitch, width * 3 * sizeof(unsigned char), height) != cudaSuccess)
        abortError("Fail buffer allocation");
    const auto buffer = flatten(buffer_, width * 3, height);
    if (cudaMemcpy2D(devBuffer, BufferPitch, buffer, width * 3 * sizeof(unsigned char),
                     width * 3 * sizeof(unsigned char), height, cudaMemcpyHostToDevice) != cudaSuccess)
        abortError("Fail buffer copy");
    free(buffer);

    // Device I
    unsigned char *devI;
    size_t IPitch;
    if (cudaMallocPitch(&devI, &IPitch, width * sizeof(unsigned char), height) != cudaSuccess)
        abortError("Fail Ix allocation");
    compute_intensities<<<dimGrid, dimBlock>>>(devBuffer, BufferPitch, width, height,
                                               devI, IPitch);
    cudaDeviceSynchronize();

    // Device result
    unsigned char *devRes;
    size_t ResPitch;
    if (cudaMallocPitch(&devRes, &ResPitch, width * 3 * sizeof(unsigned char), height) != cudaSuccess)
        abortError("Fail result allocation");

    // Device mask
    bool *devMask;
    size_t MaskPitch;
    if (cudaMallocPitch(&devMask, &MaskPitch, (2 * RADIUS + 1) * sizeof(bool), 2 * RADIUS + 1) != cudaSuccess)
        abortError("Fail mask allocation");
    const auto mask = circle_mask<RADIUS>();
    if (cudaMemcpy2D(devMask, MaskPitch, mask, (2 * RADIUS + 1) * sizeof(bool),
                     (2 * RADIUS + 1) * sizeof(bool), (2 * RADIUS + 1), cudaMemcpyHostToDevice) != cudaSuccess)
        abortError("Fail mask copy");
    free(mask);

    // Device Imax
    unsigned char *devImax;
    size_t ImaxPitch;
    if (cudaMallocPitch(&devImax, &ImaxPitch, width * sizeof(unsigned char), height) != cudaSuccess)
        abortError("Fail Imax allocation");

    get_Imax<RADIUS><<<dimGrid, dimBlock>>>(devI, IPitch, width, height,
                                                devMask, MaskPitch,devImax, ImaxPitch);
    cudaDeviceSynchronize();
    apply_filter<RADIUS><<<dimGrid, dimBlock>>>(devBuffer, BufferPitch, width, height,
                                                devRes, ResPitch, devMask, MaskPitch,
                                                devI, IPitch, devImax, ImaxPitch);
    cudaDeviceSynchronize();
    cudaFree(devI);
    cudaFree(devMask);
    cudaFree(devBuffer);

    auto res_ = (unsigned char *)malloc( height * width * 3 * sizeof(unsigned char));

    if (cudaMemcpy2D(res_, width * 3 * sizeof(unsigned char), devRes, ResPitch,
                     width * 3 * sizeof(unsigned char), height, cudaMemcpyDeviceToHost) != cudaSuccess)
        abortError("Fail result copy");
    cudaFree(devRes);

    auto res = unflatten(res_, width * 3, height);
    free(res_);

    return res;
}
