# Export Blender models to XML format for use with OpenGL.
# Assumes one material per mesh (OpenGL colors vertices rather than faces as in Blender)
# If a model uses more than one material, split into meshes and make child of parent mesh.
# Currently only handles one level of parenting.

# TODO: Export UV coordinates, bone matrices, animation data (armature deform & shape keys)

import bpy
import time
import numpy
import math


context = bpy.context
scene = context.scene

# Apply correction to vertex: swap Y & Z axes, negate Z
def vertex_correction(vDataX, vDataY, vDataZ):
    return '{0:,.4f}'.format(vDataX) + ' ' + '{0:,.4f}'.format(vDataZ) + ' ' + '{0:,.4f}'.format(-vDataY) + ' '

# Write XML file header and info
def write_header_info(file):
    headerInfo = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"               + \
                 "<exporter version=\"0.5\">\n"                               + \
                 "  <info>\n"                                                 + \
                 "    <author></author>\n"                                    + \
                 "    <created>" + time.strftime("%Y-%m-%d") + "</created>\n" + \
                 "  </info>\n"
    file.write(headerInfo)

# Write the mesh data for an object to XML file
def writeMeshData(file, item):
    mesh = item.to_mesh(scene, True, 'PREVIEW') # create new mesh from item and apply modifiers
    file.write("    <mesh id=\"" + item.name + "_mesh\">\n")

    positions = ""
    vertexCount = 0
    for vertex in mesh.vertices:
        positions += vertex_correction(vertex.co[0], vertex.co[1], vertex.co[2])
        vertexCount += 1
    file.write("      <vertex_positions id=\"" + item.name + "_positions\" count=\"" + str(vertexCount) + "\">")
    file.write(positions.rstrip() + "</vertex_positions>\n")

    normals = ""
    normalCount = 0
    for vertex in mesh.vertices:
        normals += vertex_correction(vertex.normal[0], vertex.normal[1], vertex.normal[2])
        normalCount += 1
    file.write("      <vertex_normals id=\"" + item.name + "_normals\" count=\"" + str(normalCount) + "\">")
    file.write(normals.rstrip() + "</vertex_normals>\n")

    indices = ""
    indexCount = 0
    for face in mesh.polygons:
        for vertex in face.vertices:
            indices += str(vertex) + ' '
            indexCount += 1
    file.write("      <face_indices id=\"" + item.name + "_indices\" count=\"" + str(indexCount) + "\">")
    file.write(indices.rstrip() + "</face_indices>\n")

    color = item.active_material.diffuse_color
    file.write("      <color id=\"" + item.name + "_color\"" + ">" )
    file.write('{0:,.3f}'.format(color[0])  + ' ' + '{0:,.3f}'.format(color[1]) + ' ' + '{0:,.3f}'.format(color[2]))
    file.write("</color>")

    file.write("\n    </mesh>\n")

# Export the data to XML format
def export():
    # OPEN FILE
    filename = bpy.data.filepath.rsplit(".", 1)[0]
    file     = open(filename + ".xml", 'w+')

    write_header_info(file)

    # GET MESH DATA FOR EACH OBJECT AND WRITE TO FILE
    for item in bpy.data.objects:
        if item.type == 'MESH' and item.parent == None:
            file.write("  <object id=\"" + item.name + "\">\n")

            # Write parent object's data
            writeMeshData(file, item)

            # Write the child object's data
            for child in item.children:
                writeMeshData(file, child)

            file.write("  </object>\n")

    file.write("</exporter>")

export()