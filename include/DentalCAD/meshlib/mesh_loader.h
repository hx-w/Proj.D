//
// Created by carol on 2023/11/18.
//

#ifndef DENTALCAD_MESH_LOADER_H
#define DENTALCAD_MESH_LOADER_H

#include <memory>
#include <string>
#include <vector>

namespace DentalLib {
    class DlMesh; // mesh_data.h
    using Ptr_DlMesh = std::shared_ptr<DlMesh>;

    enum class MeshLoadStatus {
        Success       = 0x00,
        FileNotFound  = 0x01,
        FileExtErr    = 0x02,
        FileOpenErr   = 0x03,
        UnImplErr     = 0x04,
        UnexpectedErr = 0x05
    };

    class MeshLoader {
    public:
        /**
         * Load [ply|obj] file to DlMesh.
         * @param filename [in] -> obj filename.
         * @param status [out] -> refer to MeshLoadStatus. no error raise.
         * @return DentalLib mesh.
         */
        static Ptr_DlMesh load(const std::string & /* filename */, MeshLoadStatus & /* status */);

        /**
         * @param str [in] string to be split.
         * @param pattern [in] split pattern.
         * @return split vector<string>.
         */
        static std::vector<std::string>
        string_split(const std::string & /* str */, const std::string & /* pattern */);

        /**
         * @brief erase WHITESPACE=" \n\r\t\f\v" or std::isspace(ch) in str, may choose inplace or not.
         * @param str [in|out] string to be trim.
         * @param inplace [in] true => result store in str, false => result returned.
         * @return trimmed string, if inplace=true, else empty.
         */
        static std::string string_strip(std::string & /* str */, bool /* inplace */ = true);
    };

    class ObjLoader : public MeshLoader {
    public:
        /**
         * Load *.obj file to DlMesh
         */
        static Ptr_DlMesh load(const std::string & /* filename */, MeshLoadStatus & /* status */);
    };

    class PlyLoader : public MeshLoader {
    public:
        /**
         * Load *.obj file to DlMesh
         */
        static Ptr_DlMesh load(const std::string & /* filename */, MeshLoadStatus & /* status */);
    };
}

#endif //DENTALCAD_OBJ_LOADER_H
