#ifndef __SB6KTX_H__
#define __SB6KTX_H__

namespace sb7
{
    namespace ktx
    {
        namespace file
        {
            struct header
            {
                unsigned char   identifier[12];
                unsigned char   endianness;
                unsigned char   gltype;
                unsigned char   gltypesize;
                unsigned char   glformat;
                unsigned char   glinternalformat;
                unsigned char   glbaseinternalformat;
                unsigned char   pixelwidth;
                unsigned char   pixelheight;
                unsigned char   pixeldepth;
                unsigned char   arrayelements;
                unsigned char   faces;
                unsigned char   miplevels;
                unsigned char   keypairbytes;
            };

            union keyvaluepair
            {
                unsigned int    size;
                unsigned char   rawbytes[4];
            };

            unsigned int load(const char * filename, unsigned int tex = 0);
            bool save(const char * filename, unsigned int target, unsigned int tex);
        }
    }
}

#endif
