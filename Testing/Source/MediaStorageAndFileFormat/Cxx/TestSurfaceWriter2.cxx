
#include "gdcmTesting.h"
#include "gdcmSurfaceWriter.h"
#include "gdcmSurfaceReader.h"
#include "gdcmFileMetaInformation.h"
#include "gdcmFilename.h"
#include "gdcmSystem.h"
#include "gdcmAttribute.h"

namespace gdcm
{

/**
  * Test surface reading and writing method.
  */
int TestSurfaceWriter2(const char *subdir, const char* filename)
{
  SurfaceReader reader;
  reader.SetFileName( filename );
  if ( !reader.Read() )
  {
    std::cerr << "Failed to read: " << filename << std::endl;
    return 1;
  }
//  std::cout << "success to read" << std::endl;

  if (reader.GetNumberOfSurfaces() < 2)
  {
    std::cerr << "File with not enough surfaces (min : 2): " << filename << std::endl;
    return 1;
  }

  // Modify data to test other writing/reading way
  SmartPointer< Segment >         segment = new Segment;
  SegmentHelper::BasicCodedEntry  processingAlgo("123", "TEST", "Test123");
  {
    SegmentReader::SegmentVector  segments = reader.GetSegments();
    Segment::SurfaceVector        tmp;
    SegmentReader::SegmentVector::iterator itSegments    = segments.begin();
    SegmentReader::SegmentVector::iterator itEndSegments = segments.end();
    for (; itSegments != itEndSegments; itSegments++)
    {
      tmp = (*itSegments)->GetSurfaces();
      Segment::SurfaceVector::iterator itSurfaces    = tmp.begin();
      Segment::SurfaceVector::iterator itEndSurfaces = tmp.end();
      for (; itSurfaces != itEndSurfaces; itSurfaces++)
      {
        SmartPointer< Surface > surf = *itSurfaces;
        surf->SetSurfaceProcessing( true );
        surf->SetSurfaceProcessingRatio( 0.42 );
        surf->SetSurfaceProcessingDescription( "Test processing" );
        surf->SetProcessingAlgorithm( processingAlgo );

        //      if (surf->GetNumberOfVectors() > 0)
        //        surf->SetNumberOfVectors( 42 );

        segment->AddSurface(surf);
      }
    }

    segment->SetSegmentDescription("Surface test");
//  segment.SetSurfaceCount( segment.GetSurfaces().size() );
  }

  // Create directory first:
  std::string tmpdir = Testing::GetTempDirectory( subdir );
  if( !System::FileIsDirectory( tmpdir.c_str() ) )
  {
    System::MakeDirectory( tmpdir.c_str() );
    //return 1;
  }
  std::string outfilename = Testing::GetTempFilename( filename, subdir );

  // Write file from content reader
  {
    SurfaceWriter writer;
    writer.SetFileName( outfilename.c_str() );
    writer.AddSegment( segment );
//  writer.SetNumberOfSurfaces( 1 );

    // Set same header to write file
    const FileMetaInformation & header = reader.GetFile().GetHeader();
    writer.GetFile().SetHeader( header );

    if( !writer.Write() )
    {
      std::cerr << "Failed to write: " << outfilename << std::endl;
      return 1;
    }
//  std::cout << "success to write" << std::endl;
  }

  SurfaceReader reader2;
  reader2.SetFileName( outfilename.c_str() );
  if ( !reader2.Read() )
  {
    std::cerr << "Failed to read again: " << outfilename << std::endl;
    return 1;
  }
//  std::cout << "success to read again" << std::endl;

  SegmentReader::SegmentVector  segments2 = reader2.GetSegments();
  if ( strcmp(segment->GetSegmentDescription(), segments2[0]->GetSegmentDescription()) != 0)
  {
    std::cerr << "Find different segment description"<< std::endl;
    return 1;
  }

  Segment::SurfaceVector        surfaces2 = segments2[0]->GetSurfaces();
  if ( segment->GetSurfaces().size() != surfaces2.size() )
  {
    std::cerr << "Find different surface count"<< std::endl;
    return 1;
  }

  Segment::SurfaceVector::iterator itSurfaces2    = surfaces2.begin();
  Segment::SurfaceVector::iterator itEndSurfaces2 = surfaces2.end();
  SegmentHelper::BasicCodedEntry processingAlgo2;
  for (; itSurfaces2 != itEndSurfaces2; itSurfaces2++)
  {
    SmartPointer< Surface > surf = *itSurfaces2;

    if (!surf->GetSurfaceProcessing())
    {
      std::cerr << "Find different surface processing"<< std::endl;
      return 1;
    }

    if ( 0.41 > surf->GetSurfaceProcessingRatio() || surf->GetSurfaceProcessingRatio() > 0.43)
    {
      std::cerr << "Find different surface processing ratio"<< std::endl;
      return 1;
    }

    if (strcmp(surf->GetSurfaceProcessingDescription(), "Test processing") != 0)
    {
      std::cerr << "Find different surface processing description"<< std::endl;
      return 1;
    }

    processingAlgo2 = surf->GetProcessingAlgorithm();
    String<> str;
    str = processingAlgo2.CV;
    processingAlgo2.CV = str.Trim();
    str = processingAlgo2.CSD;
    processingAlgo2.CSD = str.Trim();
    str = processingAlgo2.CM;
    processingAlgo2.CM = str.Trim();

    if (processingAlgo2.CV != processingAlgo.CV
     || processingAlgo2.CSD != processingAlgo.CSD
     || processingAlgo2.CM != processingAlgo.CM )
    {
      std::cerr << "Find different surface processing algorithm"<< std::endl;
      return 1;
    }
  }

  return 0;
}

}

int TestSurfaceWriter2(int argc, char *argv[])
{
  if( argc == 2 )
  {
    const char *filename = argv[1];
    return gdcm::TestSurfaceWriter2(argv[0],filename);
  }

  // else
  gdcm::Trace::DebugOff();
  gdcm::Trace::WarningOff();
  int r = 0, i = 0;
  const char *filename;
  const char * const *filenames = gdcm::Testing::GetFileNames();
  while( (filename = filenames[i]) )
  {
    r += gdcm::TestSurfaceWriter2(argv[0], filename );
    ++i;
  }

  return r;
}