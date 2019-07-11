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
#ifndef itkWaveletCoeffsInverseFFT_h
#define itkWaveletCoeffsInverseFFT_h


#include "itkForwardFFTImageFilter.h"
#include "itkInverseFFTImageFilter.h"
#include "itkFFTPadImageFilter.h"
#include "itkWaveletFrequencyForward.h"
#include "itkWaveletFrequencyInverse.h"
#include "itkWaveletFrequencyForwardUndecimated.h"
#include "itkWaveletFrequencyInverseUndecimated.h"
#include "itkWaveletFrequencyFilterBankGenerator.h"
#include "itkHeldIsotropicWavelet.h"
#include "itkVowIsotropicWavelet.h"
#include "itkSimoncelliIsotropicWavelet.h"
#include "itkShannonIsotropicWavelet.h"

#include "itkMonogenicSignalFrequencyImageFilter.h"
#include "itkVectorInverseFFTImageFilter.h"
#include "itkPhaseAnalysisSoftThresholdImageFilter.h"
#include "itkZeroDCImageFilter.h"

#include "itkImage.h"
#include "itkCastImageFilter.h"
#include "itkNumberToString.h"
#include <string>


namespace itk
{

template <typename TImage> class WaveletCoeffsInverseFFT: public ImageToImageFilter<TImage, TImage>
    {

    public:
        ITK_DISALLOW_COPY_AND_ASSIGN(WaveletCoeffsInverseFFT);

        using Self = WaveletCoeffsInverseFFT;
        using Superclass = ImageToImageFilter<TImage, TImage>;
        using Pointer = SmartPointer<Self>;
        using ConstPointer = SmartPointer<const Self>;

        itkNewMacro(Self);
        itkTypeMacro(WaveletCoeffsInverseFFT, ImageToImageFilter);

        using ImageType = TImage;
        using IntType = unsigned int;
        static constexpr unsigned int ImageDimension = ImageType::ImageDimension;
        using WaveletScalarType = double;
        using ImageFloatType = Image< float, ImageDimension >;

        itkGetMacro(Levels, IntType);
        itkSetMacro(Levels, IntType);
        itkGetMacro(HighPassSubBands, IntType);
        itkSetMacro(HighPassSubBands, IntType);

        itkGetMacro(OutputIndex, IntType);
        itkSetMacro(OutputIndex, IntType);


    protected:
        WaveletCoeffsInverseFFT();

    protected:
        using FFTPadType = FFTPadImageFilter< ImageType >;
        using ZeroDCType = ZeroDCImageFilter< ImageType >;
        using FFTForwardType = ForwardFFTImageFilter< typename ZeroDCType::OutputImageType >;
        using ComplexImageType = typename FFTForwardType::OutputImageType;

        using WaveletFunctionType = SimoncelliIsotropicWavelet< WaveletScalarType, ImageDimension>;
        using WaveletFilterBankType = WaveletFrequencyFilterBankGenerator< ComplexImageType, WaveletFunctionType >;
        using ForwardWaveletType = WaveletFrequencyForward< ComplexImageType, ComplexImageType, WaveletFilterBankType >;

        using InverseFFTType = InverseFFTImageFilter< ComplexImageType, ImageType >;
        using ChangeInformationType = ChangeInformationImageFilter< ImageType >;
        using CastFloatType = CastImageFilter< ImageType, ImageFloatType>;


        void GenerateData() override;
        void PrintSelf( std::ostream& os, Indent indent) const override;

    private:
        typename FFTPadType::Pointer m_FFTPadFilter;
        typename ZeroDCType::Pointer m_ZeroDCFilter;
        typename FFTForwardType::Pointer m_ForwardFFTFilter;
        typename InverseFFTType::Pointer m_InverseFFTFilter;
        typename ForwardWaveletType::Pointer m_ForwardWaveletFilter;
        typename ChangeInformationType::Pointer m_ChangeInformationFilter;
        typename CastFloatType::Pointer m_CastFloatFilter;

        unsigned int m_Levels;
        unsigned int m_HighPassSubBands;
        unsigned int m_OutputIndex;


    };

}
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkWaveletCoeffsInverseFFT.hxx"
#endif

#endif
