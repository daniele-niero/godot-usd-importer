#pragma once

#include <godot_cpp/classes/node3d.hpp>

#include <pxr/usd/usd/prim.h>
#include "pxr/usd/usdGeom/tokens.h"
#include <pxr/usd/usdGeom/mesh.h>
#include <pxr/usd/usdGeom/primvar.h>

#include <vector>


namespace godot_usd_importer {


struct GdMeshArrays {
    // Arrays for the new Godot mesh
    godot::PackedVector3Array vertices;
    godot::PackedVector3Array normals;
    godot::PackedVector2Array uvs;
    godot::PackedColorArray   colors;
    godot::PackedInt32Array   indices;
};

struct MeshData 
{
    MeshData(const pxr::UsdGeomMesh& usd_mesh);

    pxr::VtArray<pxr::GfVec3f>  usd_points;
    pxr::VtArray<pxr::GfVec3f>  usd_normals;
    pxr::VtArray<int>           usd_face_vertex_counts;
    pxr::VtArray<int>           usd_face_vertex_indices;
    pxr::TfToken                usd_normal_interp;
    
    pxr::VtArray<
        pxr::UsdGeomPrimvar>    usd_uv_primvars;
    
    enum GodotVertexStrategy {
        Unknown,
        Split,
        Merge
    };
    
    bool is_valid() {
        return _is_valid;
    }

    bool to_godot_arrays(GdMeshArrays &gd_arrays, const double &meters_per_unit, bool zup_to_yup) const;
    
    GodotVertexStrategy get_vertex_strategy() const;
    bool reverse_winding() const {
        return (usd_face_orientation_token == pxr::UsdGeomTokens->rightHanded);
    }
    
    
private:
    pxr::TfToken usd_face_orientation_token;
    bool _is_valid = false;

    std::vector<int> get_face_primvar_indices(
            const std::vector<int> &current_face_vertex_indices, 
            const int current_face_vertex_count, 
            const int vertex_offset,
            const int usd_face_idx,
            const pxr::TfToken &interpolation
    ) const;
};





}