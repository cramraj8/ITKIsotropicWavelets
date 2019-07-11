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
#ifndef itkWaveletCoeffsInverseFFT_hxx
#define itkWaveletCoeffsInverseFFT_hxx

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


namespace itk{

    template<typename TImage> WaveletCoeffsInverseFFT<TImage> :: WaveletCoeffsInverseFFT()
    // : m_Level(4)
    {
        m_Levels = 4;
        m_HighPassSubBands = 3;
        m_OutputIndex = 1;


        m_FFTPadFilter = FFTPadType::New();
        m_ZeroDCFilter = ZeroDCType::New();
        m_ForwardFFTFilter = FFTForwardType::New();
        m_ForwardWaveletFilter = ForwardWaveletType::New();
        m_InverseFFTFilter = InverseFFTType::New();
        m_ChangeInformationFilter = ChangeInformationType::New();
        m_CastFloatFilter = CastFloatType::New();


        m_ForwardWaveletFilter-> SetHighPassSubBands( m_HighPassSubBands );
        m_ForwardWaveletFilter-> SetLevels( m_Levels );


        m_ZeroDCFilter-> SetInput( m_FFTPadFilter-> GetOutput() );
        m_ForwardFFTFilter-> SetInput( m_ZeroDCFilter-> GetOutput() );
        m_ForwardWaveletFilter-> SetInput( m_ForwardFFTFilter-> GetOutput() );
        m_InverseFFTFilter-> SetInput( m_ForwardWaveletFilter-> GetOutputs()[m_OutputIndex] ); // This API is strange.
        m_ChangeInformationFilter-> SetInput( m_InverseFFTFilter-> GetOutput() );
        m_CastFloatFilter-> SetInput( m_ChangeInformationFilter-> GetOutput() );

    }


    template<typename TImage> void WaveletCoeffsInverseFFT<TImage> :: GenerateData()
    {

        typename ImageType::Pointer input = ImageType::New();
        input-> Graft(const_cast<ImageType*> ( this-> GetInput() ));
        m_FFTPadFilter-> SetInput( input );
        // m_ZeroDCFilter-> Update();

        m_ForwardWaveletFilter-> SetHighPassSubBands( this-> m_HighPassSubBands );
        m_ForwardWaveletFilter-> SetLevels( this-> m_Levels );
        // m_ForwardWaveletFilter-> Update();

        m_CastFloatFilter-> GraftOutput( this-> GetOutput() );
        m_CastFloatFilter-> Update();
        this-> GraftOutput( m_CastFloatFilter-> GetOutput() );

    }


    template<typename TImage> void WaveletCoeffsInverseFFT<TImage> :: PrintSelf( std::ostream& os, Indent indent ) const
    {

        Superclass::PrintSelf(os, indent);
        os  << indent
            << "==============================================================\n"
            << "  ============== Composite Filter Specifications ===============\n"
            << " Levels : " << this-> m_Levels
            << "\n HighPassSubBands : " << this-> m_HighPassSubBands
            << std::endl;

        // auto sizeAfterPad = this-> m_FFTPadFilter-> GetOutput()-> GetLargestPossibleRegion().GetSize();
        // unsigned int scaleFactor = 2;
        // // unsigned int maxNumberOfLevels = itk::utils::ComputeMaxNumberOfLevels(sizeAfterPad, scaleFactor);
        // os  << indent
        //     << "Original Size:" << sizeAfterPad
        //     // Something wring here
        //     << std::endl;
        os << indent << " Composite Filter was constructed !! " << std::endl;

    }

}
#endif
