#include <ROOT/RNTupleImporter.hxx>

#include <TROOT.h>

#include <cstdio>
#include <iostream>
#include <string>

#include <unistd.h>

#include "util.h"

using RNTupleImporter = ROOT::Experimental::RNTupleImporter;

void Usage(char *progname)
{
   std::cout << "Usage: " << progname << " -o <ntuple-path> -c <compression> [-m(t)] <H1 root file>"
             << std::endl;
}

int main(int argc, char **argv)
{
   std::string inputFile = "h1dstX10.root";
   std::string outputPath = ".";
   int compressionSettings = 0;
   std::string compressionShorthand = "none";
   std::string treeName = "h42";

   int c;
   while ((c = getopt(argc, argv, "hvi:o:c:m")) != -1) {
      switch (c) {
      case 'h':
      case 'v':
         Usage(argv[0]);
         return 0;
      case 'i':
         inputFile = optarg;
         break;
      case 'o':
         outputPath = optarg;
         break;
      case 'c':
         compressionSettings = GetCompressionSettings(optarg);
         compressionShorthand = optarg;
         break;
      case 'm':
         ROOT::EnableImplicitMT();
         break;
      default:
         fprintf(stderr, "Unknown option: -%c\n", c);
         Usage(argv[0]);
         return 1;
      }
   }
   std::string dsName = "h1dstX10";
   std::string outputFile = outputPath + "/" + dsName + "~" + compressionShorthand + ".ntuple";

   unlink(outputFile.c_str());
   auto importer = RNTupleImporter::Create(inputFile, treeName, outputFile).Unwrap();
   auto options = importer->GetWriteOptions();
   options.SetCompression(compressionSettings);
   importer->SetWriteOptions(options);
   importer->Import().ThrowOnError();

   return 0;
}
