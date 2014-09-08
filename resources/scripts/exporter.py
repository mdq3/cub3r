# Export Blender models to XML format for use with OpenGL.
# Assumes one material per mesh (OpenGL colors vertices rather than faces as in Blender)
# If a model uses more than one material, split into meshes and make child of parent mesh.
# Currently only handles one level of parenting.

# TODO: Export bone matrices, animation data (armature deforms & shape keys)

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
                 "<exporter version=\"0.7\">\n"                               + \
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
    normals = ""
    uvCoords = ""
    
    vertexCount = 0
    normalCount = 0
    uvCount = 0
    
    for face in mesh.polygons:
        for vertexIndex in face.vertices:
            vertex = mesh.vertices[vertexIndex]
            positions += vertex_correction(vertex.co[0], vertex.co[1], vertex.co[2])
            normals += vertex_correction(vertex.normal[0], vertex.normal[1], vertex.normal[2])
            vertexCount += 1
            normalCount += 1
        for loopIndex in face.loop_indices:
            uv_layer = mesh.uv_layers.active.data
            uvCoords += '{0:,.4f}'.format(uv_layer[loopIndex].uv.x) + ' ' + '{0:,.4f}'.format(1 - uv_layer[loopIndex].uv.y) + ' '
            uvCount += 1
           
    file.write("      <vertex_positions id=\"" + item.name + "_positions\" count=\"" + str(vertexCount) + "\">")
    file.write(positions.rstrip() + "</vertex_positions>\n")

    file.write("      <vertex_normals id=\"" + item.name + "_normals\" count=\"" + str(normalCount) + "\">")
    file.write(normals.rstrip() + "</vertex_normals>\n")
    
    file.write("      <uv_coords id=\"" + item.name + "_uv\" count=\"" + str(uvCount) +  "\">")
    file.write(uvCoords.rstrip() + "</uv_coords>\n")
    
    texture_image_path = item.data.uv_textures[0].data.values()[0].image.filepath
    file.write("      <texture_image id=\"" + item.name + "_texture\">")
    file.write("resources/" + texture_image_path[5:] + "</texture_image>\n")

    file.write("    </mesh>\n")

# Export the data to XML format
def export():
    # OPEN FILE
    filename = bpy.data.filepath.rsplit(".", 1)[0]
    file     = open(filename + ".xml", 'w+')

    write_header_info(file)

    # GET MESH DATA FOR EACH OBJECT AND WRITE TO FILE
    for item in bpy.data.objects:
        if item.type == 'MESH' and item.parent == None: # item type mesh, and top level parent
            file.write("  <object id=\"" + item.name + "\">\n")

            # Write parent object's data
            writeMeshData(file, item)

            # Write the child object's data
            #for child in item.children:
            #    writeMeshData(file, child)

            file.write("  </object>\n")

    file.write("</exporter>")

export()