#ifndef __itkNeighborhoodConverter_h
#define __itkNeighborhoodConverter_h

#include "itkNeighborhood.h"
#include "itkSize.h"
#include "itkOffset.h"
#include <vector>
#include "itkVector.h"

namespace itk {

/** \class NeighborhoodConverter
* \brief 
*
**/

template< class TNeighborhood=Neighborhood<bool, 2>, class TImage=Image<unsigned char, 2> >
class ITK_EXPORT NeighborhoodConverter
{
public:  
  typedef TNeighborhood NeighborhoodType;
  typedef TImage        ImageType;

  typedef typename ImageType::Pointer       ImagePointer;
  typedef typename ImageType::ConstPointer  ImageConstPointer;
  typedef typename ImageType::PixelType     PixelType;
  typedef typename ImageType::SizeType      SizeType;
  typedef typename ImageType::SizeValueType SizeValueType;
  typedef typename ImageType::OffsetType    OffsetType;
  
  typedef typename NeighborhoodType::Iterator       Iterator;
  typedef typename NeighborhoodType::ConstIterator  ConstIterator;
  typedef typename NeighborhoodType::RadiusType     RadiusType;
  
  /** External support for dimensionality. */
  itkStaticConstMacro(ImageDimension, unsigned int, ImageType::ImageDimension);

  /**
   * Create a structuring element based on a binary image.
   */
  static NeighborhoodType GetNeighborhood( ImageConstPointer image,
                  const PixelType & foreground=NumericTraits< typename ImageType::PixelType >::max() );

  /** return an itk::Image from the structuring element. Background defaults to
   * NumericTraits< PixelType >::Zero and foreground to
   * NumericTraits< PixelType >::max()
   */
  static ImagePointer GetImage( const NeighborhoodType & reighborhood,
                  const PixelType & foreground=NumericTraits< typename ImageType::PixelType >::max(),
                  const PixelType & background=NumericTraits< typename ImageType::PixelType >::Zero );

};
} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkNeighborhoodConverter.txx"
#endif



#endif
