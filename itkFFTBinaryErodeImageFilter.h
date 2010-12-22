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
#ifndef __itkFFTBinaryErodeImageFilter_h
#define __itkFFTBinaryErodeImageFilter_h

#include "itkKernelImageFilter.h"

namespace itk
{
/**
 * \class FFTBinaryErodeImageFilter
 * \brief FFT based binary dilation
 *
 * This filter is a lot more efficient when ITK is built with FFTW support.
 *
 * \author Gaetan Lehmann. Biologie du Developpement et de la Reproduction, INRA de Jouy-en-Josas, France.
 *
 * \sa MorphologyImageFilter, GrayscaleErodeImageFilter, GrayscaleErodeImageFilter
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */

template< class TInputImage, class TOutputImage, class TKernel >
class ITK_EXPORT FFTBinaryErodeImageFilter:
  public KernelImageFilter< TInputImage, TOutputImage, TKernel >
{
public:
  /** Standard class typedefs. */
  typedef FFTBinaryErodeImageFilter                   Self;
  typedef KernelImageFilter< TInputImage, TOutputImage, TKernel > Superclass;
  typedef SmartPointer< Self >                                    Pointer;
  typedef SmartPointer< const Self >                              ConstPointer;

  /** Standard New method. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(FFTBinaryErodeImageFilter,
               KernelImageFilter);

  typedef TInputImage                          InputImageType;
  typedef TOutputImage                         OutputImageType;
  typedef typename InputImageType::Pointer     InputImagePointer;
  typedef typename OutputImageType::RegionType OutputImageRegionType;

  /** Declaration of pixel type. */
  typedef typename TInputImage::PixelType InputPixelType;
  typedef typename TInputImage::PixelType OutputPixelType;

  /** Kernel typedef. */
  typedef TKernel KernelType;

  itkStaticConstMacro(ImageDimension, unsigned int, TInputImage::ImageDimension);

  /** Set the value in the image to consider as "foreground". Defaults to
   * maximum value of InputPixelType. */
  itkSetMacro(ForegroundValue, InputPixelType);

  /** Get the value in the image considered as "foreground". Defaults to
   * maximum value of InputPixelType. */
  itkGetConstMacro(ForegroundValue, InputPixelType);

  /** Set the value in eroded part of the image. Defaults to zero */
  itkSetMacro(BackgroundValue, InputPixelType);

  /** Set the value in eroded part of the image. Defaults to zero */
  itkGetConstMacro(BackgroundValue, InputPixelType);
  
protected:
  FFTBinaryErodeImageFilter();
  ~FFTBinaryErodeImageFilter() {}
  void PrintSelf(std::ostream & os, Indent indent) const;

  void  BeforeThreadedGenerateData();
  void ThreadedGenerateData(const OutputImageRegionType & outputRegionForThread,
                            int threadId);

private:
  FFTBinaryErodeImageFilter(const Self &); //purposely not
                                                       // implemented
  void operator=(const Self &);                        //purposely not
                                                       // implemented

  InputPixelType m_ForegroundValue;
  InputPixelType m_BackgroundValue;

}; // end of class
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkFFTBinaryErodeImageFilter.txx"
#endif

#endif
