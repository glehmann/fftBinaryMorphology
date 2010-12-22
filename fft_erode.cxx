#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkSimpleFilterWatcher.h"

#include "itkImage.h"
#include "itkFlatStructuringElement.h"
#include "itkFFTBinaryErodeImageFilter.h"

int main(int argc, char * argv[])
{

  if( argc != 6 )
    {
    std::cerr << "usage: " << argv[0] << " intput output radius fg bg" << std::endl;
    std::cerr << " input: the input image" << std::endl;
    std::cerr << " output: the output image" << std::endl;
    // std::cerr << "  : " << std::endl;
    exit(1);
    }

  const int dim = 3;
  
  typedef unsigned char PType;
  typedef itk::Image< PType, dim > IType;

  typedef itk::ImageFileReader< IType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

  typedef itk::FlatStructuringElement< dim > KernelType;
  KernelType::RadiusType rad;
  rad.Fill( atoi(argv[3]) );
  KernelType ball = KernelType::Ball( rad );
  
  typedef itk::FFTBinaryErodeImageFilter< IType, IType, KernelType > FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( reader->GetOutput() );
  filter->SetKernel( ball );
  filter->SetForegroundValue( atoi(argv[4]) );
  filter->SetBackgroundValue( atoi(argv[5]) );

  itk::SimpleFilterWatcher watcher(filter, "filter");

  typedef itk::ImageFileWriter< IType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( filter->GetOutput() );
  writer->SetFileName( argv[2] );
  writer->Update();

  return 0;
}

