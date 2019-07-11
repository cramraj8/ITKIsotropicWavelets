cmake_minimum_required(VERSION 3.5)
project(IsotropicWavelets-runWaveletCoeffsInverseFFT_example)

find_package(ITK REQUIRED COMPONENTS
  ITKCommon
  ITKIOImageBase
  ITKImageGrid
  ITKImageIntensity
  ITKImageStatistics
  IsotropicWavelets
  ITKImageIO
  CONFIG
)
include(${ITK_USE_FILE})
include_directories(SYSTEM ${ITK_INCLUDE_DIRS})

add_executable(runWaveletCoeffsInverseFFT_example runWaveletCoeffsInverseFFT_example.cxx)
target_link_libraries(runWaveletCoeffsInverseFFT_example PUBLIC ${ITK_LIBRARIES})
