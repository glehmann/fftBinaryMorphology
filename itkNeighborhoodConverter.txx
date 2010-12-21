
#ifndef __itkNeighborhoodConverter_txx
#define __itkNeighborhoodConverter_txx

#include "itkNeighborhoodConverter.h"

namespace itk
{

template<class TNeighborhood, class TImage>
TNeighborhood
NeighborhoodConverter<TNeighborhood, TImage>::
GetNeighborhood( ImageConstPointer image, const PixelType & foreground )
{
  image->Update();
  RadiusType size = image->GetLargestPossibleRegion().GetSize();
  Index< ImageDimension > centerIdx;
  for( int i=0; i<(int)ImageDimension; i++ )
    {
    // TODO: throw an exception if size is not odd
    size[i] = size[i] / 2;
    centerIdx[i] = size[i];
    }
  NeighborhoodType res = NeighborhoodType();
  res.SetRadius( size );

  for(int i=0; i<(int)res.Size(); i++ )
    {
    res[i] = image->GetPixel( centerIdx + res.GetOffset( i ) );
    }

  return res;
}

template<class TNeighborhood, class TImage>
typename NeighborhoodConverter<TNeighborhood, TImage>::ImagePointer
NeighborhoodConverter<TNeighborhood, TImage>::
GetImage( const NeighborhoodType & neighborhood, const PixelType & foreground, const PixelType & background )
{
  typename ImageType::Pointer image = ImageType::New();
  typename ImageType::RegionType region;
  RadiusType size = neighborhood.GetRadius();
  Index< ImageDimension > centerIdx;
  for( int i=0; i<(int)ImageDimension; i++ )
    {
    centerIdx[i] = size[i];
    size[i] = 2*size[i] + 1;
    }
  region.SetSize( size );
  image->SetRegions( region );
  image->Allocate();

  // std::cout << neighborhood.GetRadius() << std::endl;
  // image->Print( std::cout );

  for(int i=0; i<(int)(neighborhood.Size()); i++ )
    {
    if( neighborhood[i] )
      {
      image->SetPixel( centerIdx+neighborhood.GetOffset( i ), foreground );
      }
    else
      {
      image->SetPixel( centerIdx+neighborhood.GetOffset( i ), background );
      }
    }

  return image;

}

}

#endif
