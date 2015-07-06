import ocls_framework as ocls

start                   = ocls.c_api.oclsAnalyserStart
end                     = ocls.c_api.oclsAnalyserEnd
timeline                = ocls.c_api.oclsAnalyserRead
kernel_to_string        = ocls.c_api.oclsAnalyserKernelRead
memory_to_string        = ocls.c_api.oclsAnalyserMemoryRead
kernel                  = ocls.c_api.oclsAnalyserKernel
memory                  = ocls.c_api.oclsAnalyserMemory
