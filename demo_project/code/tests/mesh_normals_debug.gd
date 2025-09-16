@tool
extends MeshInstance3D

@export var normal_length: float = 0.5
@export var update_every_frame := true

var immediate := ImmediateMesh.new()
var mi := MeshInstance3D.new()

func _ready():
    #if mesh == null:
        #return
    mi.mesh = immediate
    add_child(mi)

func _process(delta: float) -> void:
    if update_every_frame:
        _draw_normals()

func _draw_normals():
    if mesh == null:
        print('no mesh')
        return

    var mdt := MeshDataTool.new()
    if mdt.create_from_surface(mesh, 0) != OK:
        return

    var mat := StandardMaterial3D.new()
    mat.shading_mode = BaseMaterial3D.SHADING_MODE_UNSHADED
    mat.albedo_color = Color.RED

    immediate.clear_surfaces()  # ✅ correct way in Godot 4

    immediate.surface_begin(Mesh.PRIMITIVE_LINES, mat)

    for i in range(mdt.get_vertex_count()):
        var pos = mdt.get_vertex(i)
        var normal = mdt.get_vertex_normal(i)
        immediate.surface_add_vertex(pos)
        immediate.surface_add_vertex(pos + normal * normal_length)

    immediate.surface_end()
