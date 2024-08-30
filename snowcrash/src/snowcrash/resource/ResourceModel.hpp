
#pragma once

#include <snowcrash/core/Core.hpp>
#include <snowcrash/resource/Resource.hpp>
#include <snowcrash/types/ArrayList.hpp>
#include <snowcrash/graphics/Model.hpp>

namespace SC
{
    class ResourceModel : public Resource
    {
    public:
        explicit ResourceModel(u32 hash, Model *model);
        ~ResourceModel();

        const Model *GetModel() { return m_model; }

    private:
        const Model *m_model;
    };

    class ResourceModelLoader : public ResourceLoader
    {
    public:
        void LoadResource(Resource **resource, String path) override;
    };
} // namespace SC
