
#include "ResourceModel.hpp"

// <3 thank you
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
namespace SC
{
    ResourceModel::ResourceModel(u32 hash, Model *model)
        : Resource(hash), m_model(model) {}

    ResourceModel::~ResourceModel()
    {
        delete m_model;
    }

    void ResourceModelLoader::LoadResource(Resource **resource, String path)
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, path.c_str()))
        {
            // SC_WARN("tinyobj loader error: %s\n warn %s\n", warn.c_str(), err.c_str());
            error = err.c_str();
            return;
        }

        Model *model = new Model();

        for (const auto &shape : shapes)
        {
            model->m_vertices.Resize(shape.mesh.indices.size());
            model->m_indicies.Resize(shape.mesh.indices.size());

            for (const auto &index : shape.mesh.indices)
            {
                ModelVertex vertex{};

                vertex.position = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]};

                // we need to flip the texture coordnates
                vertex.texCoord = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    1.0f - attrib.texcoords[2 * index.texcoord_index + 1]};

                vertex.color = {1.0f, 1.0f, 1.0f};

                vertex.normal = {
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2],
                };

                model->m_vertices.Add(vertex);
                model->m_indicies.Add(model->m_indicies.GetIndex());
            }
        }

        ResourceModel *rmodel = new ResourceModel(path.Hash(), model);
        *resource = static_cast<Resource *>(rmodel);
    }
} // namespace SC
