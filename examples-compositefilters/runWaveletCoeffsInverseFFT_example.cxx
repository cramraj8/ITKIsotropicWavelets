/*=========================================================================
 *
 *  Copyright Pablo Hernandez-Cerdan
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/


// #include "itkForwardFFTImageFilter.h"
// #include "itkInverseFFTImageFilter.h"
// #include "itkFFTPadImageFilter.h"
// #include "itkWaveletFrequencyForward.h"
// #include "itkWaveletFrequencyInverse.h"
// #include "itkWaveletFrequencyForwardUndecimated.h"
// #include "itkWaveletFrequencyInverseUndecimated.h"
// #include "itkWaveletFrequencyFilterBankGenerator.h"
// #include "itkHeldIsotropicWavelet.h"
// #include "itkVowIsotropicWavelet.h"
// #include "itkSimoncelliIsotropicWavelet.h"
// #include "itkShannonIsotropicWavelet.h"

// #include "itkMonogenicSignalFrequencyImageFilter.h"
// #include "itkVectorInverseFFTImageFilter.h"
// #include "itkPhaseAnalysisSoftThresholdImageFilter.h"
// #include "itkZeroDCImageFilter.h"

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
// #include "itkCastImageFilter.h"
#include "itkNumberToString.h"
#include <string>


#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include "itkWaveletCoeffsInverseFFT.h"


std::string
AppendToFilenameRiesz(const std::string& filename, const std::string & appendix)
{
  std::size_t foundDot = filename.find_last_of('.');
  return filename.substr( 0, foundDot ) + appendix + filename.substr( foundDot );
}


int main( int argc, char* argv[] )
{

    if (argc < 6)
    {
        std::cerr << "Usage : " << std::endl;
        std::cerr << argv[0] << " inputImageFile outputImageFile levels bands outputIndex" << std::endl;
        return EXIT_FAILURE;
    }

    using ImageType = itk::Image<float, 3>;
    using ReaderType = itk::ImageFileReader<ImageType>;

    ReaderType :: Pointer reader = ReaderType::New();
    reader-> SetFileName( argv[1] );

    using FilterType = itk::WaveletCoeffsInverseFFT<ImageType>;
    FilterType :: Pointer filter = FilterType::New();
    filter-> SetInput( reader-> GetOutput() );
    // filter-> Print(std::cout); // Debugging line
    filter-> SetLevels(atoi(argv[3])); // ASCII to Integer
    filter-> SetHighPassSubBands(atoi(argv[4]));
    filter-> SetOutputIndex(atoi(argv[5]));


    itk::NumberToString< unsigned int > n2s;
    using WriterType = itk::ImageFileWriter<ImageType>;
    WriterType :: Pointer writer = WriterType::New();
    writer-> SetInput( filter-> GetOutput() );
    std::string appendString = "_L" + n2s(4) + "_B" + n2s(3) + "_S" + n2s(0.2);
    std::string outputFile = AppendToFilenameRiesz(argv[2], appendString);
    writer-> SetFileName( outputFile );
    writer-> UseCompressionOn ();

    try
    {
        writer-> Update();
    }
    catch ( itk::ExceptionObject & e )
    {
        std::cerr << "Error : " << e << std::endl;
    }

    return EXIT_SUCCESS;

}
