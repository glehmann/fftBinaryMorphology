/*=========================================================================
 *
 *  Copyright Insight Software Consortium
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
#ifndef __itkFFTBinaryDilateImageFilter_txx
#define __itkFFTBinaryDilateImageFilter_txx

#include "itkBinaryThresholdImageFilter.h"
#include "itkNeighborhoodConverter.h"
#include "itkFFTConvolutionImageFilter.h"
#include "itkProgressAccumulator.h"
#include "itkProgressReporter.h"

namespace itk
{
template< class TInputImage, class TOutputImage, class TKernel >
FFTBinaryDilateImageFilter< TInputImage, TOutputImage, TKernel >
::FFTBinaryDilateImageFilter()
{
  m_ForegroundValue = NumericTraits< InputPixelType >::max();
}

template< class TInputImage, class TOutputImage, class TKernel >
void
FFTBinaryDilateImageFilter< TInputImage, TOutputImage, TKernel >
::GenerateData()
{
  // Allocate the outputs
  this->AllocateOutputs();

  typedef Image< unsigned char, ImageDimension > InternalImageType;
  typedef Image< float, ImageDimension >         RealInternalImageType;
  
  ProgressAccumulator::Pointer progress = ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);

  // input should be 0 or 1 only
  typedef BinaryThresholdImageFilter< InputImageType, InternalImageType > InputThresholdType;
  typename InputThresholdType::Pointer ith = InputThresholdType::New();
  ith->SetInput( this->GetInput() );
  ith->SetLowerThreshold( m_ForegroundValue );
  ith->SetUpperThreshold( m_ForegroundValue );
  ith->SetInsideValue( 1 );
  ith->SetOutsideValue( 0 );
  
  typedef NeighborhoodConverter< KernelType, InternalImageType > NeighborhoodConverterType;
  typename InternalImageType::Pointer kernel = NeighborhoodConverterType::GetImage( this->GetKernel() );
  
  typedef FFTConvolutionImageFilter< InternalImageType, InternalImageType, RealInternalImageType >  ConvolutionType;
  typename ConvolutionType::Pointer conv = ConvolutionType::New();
  conv->SetInput( ith->GetOutput() );
  conv->SetKernelImage( kernel );
  conv->SetNormalize( false );
  conv->SetPadMethod( ConvolutionType::ZERO );
  
  typedef BinaryThresholdImageFilter< RealInternalImageType, OutputImageType > OutputThresholdType;
  typename OutputThresholdType::Pointer oth = OutputThresholdType::New();
  oth->SetInput( conv->GetOutput() );
  oth->SetLowerThreshold( 0.5 );
  oth->SetUpperThreshold( NumericTraits< float >::max() );
  oth->SetInsideValue( m_ForegroundValue );
  oth->SetOutsideValue( 0 );

  /** set up the minipipeline */
  progress->RegisterInternalFilter(ith, .3f);
  progress->RegisterInternalFilter(conv, .3f);
  progress->RegisterInternalFilter(oth, .3f);
  
  /** execute the minipipeline */
  oth->GraftOutput( this->GetOutput() );
  oth->Update();
  this->GraftOutput( oth->GetOutput() );

//   ImageRegionConstIterator< InputImageType > inIt =
//   ImageRegionConstIterator< InputImageType >( this->GetInput(),
//                                             this->GetOutput()->GetRequestedRegion() );
//   // iterator on output image
//   ImageRegionIterator< OutputImageType > outIt =
//   ImageRegionIterator< OutputImageType >( this->GetOutput(),
//                                         this->GetOutput()->GetRequestedRegion() );
//   outIt.GoToBegin();
//   inIt.GoToBegin();
// 
//   ProgressReporter progress2(this, 0, this->GetOutput()->GetRequestedRegion().GetNumberOfPixels(), 20, 0.9, 0.1);
//   while ( !outIt.IsAtEnd() )
//  {
// if ( outIt.Get() != m_ForegroundValue )
//   {
//   outIt.Set( static_cast< OutputPixelType >( inIt.Get() ) );
//   }
// ++outIt;
// ++inIt;
// progress2.CompletedPixel();
// }

  // the end !
}

template< class TInputImage, class TOutputImage, class TKernel >
void
FFTBinaryDilateImageFilter< TInputImage, TOutputImage, TKernel >
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "ForegroundValue: "
     << static_cast< typename NumericTraits< InputPixelType >::PrintType >( m_ForegroundValue ) << std::endl;
}
} // end namespace itk
#endif
