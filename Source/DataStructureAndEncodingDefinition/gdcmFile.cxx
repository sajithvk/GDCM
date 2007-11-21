/*=========================================================================

  Program: GDCM (Grass Root DICOM). A DICOM library
  Module:  $URL$

  Copyright (c) 2006-2007 Mathieu Malaterre
  Copyright (c) 1993-2005 CREATIS
  All rights reserved.
  See Copyright.txt or http://gdcm.sourceforge.net/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "gdcmFile.h"

#include "gdcmSwapper.txx"
#include "gdcmDataSet.h"
#include "gdcmExplicitDataElement.h"
#include "gdcmImplicitDataElement.h"
#include "gdcmValue.h"

#include "gdcmIOSerialize.h"
#include "gdcmValue.h"
#include "gdcmItem.h"
#include "gdcmSequenceOfItems.h"

#include "gdcmDeflateStream.h"

namespace gdcm
{

std::istream &File::Read(std::istream &is) 
{
  bool haspreamble = true;
  try
    {
    P.Read( is );
    }
  catch( std::exception &ex )
    {
    // return to beginning of file, hopefully this file is simply missing preamble
    is.seekg(0, std::ios::beg);
    haspreamble = false;
    }
  catch( ... )
    {
    abort();
    }

  bool hasmetaheader = true;
  try
    {
    if( haspreamble )
      {
      try
        {
        Header.Read( is );
        }
      catch( std::exception &ex )
        {
        // Weird implicit meta header:
        is.seekg(128+4, std::ios::beg );
        Header.ReadCompat(is);
        }
      }
    else
      Header.ReadCompat(is);
    }
  catch( std::exception &ex )
    {
    // Same player play again:
    is.seekg(0, std::ios::beg );
    hasmetaheader = false;
    }
  catch( ... )
    {
    // Ooops..
    abort();
    }

  const TransferSyntax &ts = Header.GetDataSetTransferSyntax();
  //std::cerr << ts.GetNegociatedType() << std::endl;
  //std::cerr << TransferSyntax::GetTSString(ts) << std::endl;
  // Special case where the dataset was compressed using the deflate
  // algorithm
  if( ts == TransferSyntax::DeflatedExplicitVRLittleEndian )
    {
    /*
    std::ofstream of("deflat.gz");
    char one_char;
    while (is.get(one_char))
    of.put(one_char);
    of.close();
    */
    gzistream gzis(is.rdbuf());
    // FIXME: we also know in this case that we are dealing with Explicit:
    assert( ts.GetNegociatedType() == TransferSyntax::Explicit );
    IOSerialize<SwapperNoOp>::Read(gzis,DS);
    is.seekg(0, std::ios::end);
    is.peek();

    return is;
    }
  if( ts.GetNegociatedType() == TransferSyntax::Implicit )
    {
    DS.SetType( TransferSyntax::Implicit );
    }
  if( ts.GetSwapCode() == SwapCode::BigEndian )
    {
    //US-RGB-8-epicard.dcm is big endian
    IOSerialize<SwapperDoOp>::Read(is,DS);
    }
  else
    {
    IOSerialize<SwapperNoOp>::Read(is,DS);
    }

  return is;
}

std::ostream const &File::Write(std::ostream &os) const
{

  P.Write(os);

  Header.Write(os);

  const TransferSyntax &ts = Header.GetDataSetTransferSyntax();
  if( ts == TransferSyntax::DeflatedExplicitVRLittleEndian )
    {
    gzostream gzos(os.rdbuf());
    assert( ts.GetNegociatedType() == TransferSyntax::Explicit );
    IOSerialize<SwapperNoOp>::Write(gzos,DS);

    return os;
    }
  else
    {
    //std::cerr << "Write dataset: " << DS.GetNegociatedType() << std::endl;
    IOSerialize<SwapperNoOp>::Write(os,DS);
    }

  return os;
}

} // end namespace gdcm

