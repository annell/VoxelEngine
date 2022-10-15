#pragma once

#define OGT_VOX_IMPLEMENTATION
#include "../external/vox/ogt_vox.h"
#include "Chunk.h"

#include "Cube.h"
#include <cstdio>
#include <map>
#include <string>

namespace voxie::ModelLoader {
    namespace internal {

        // a helper function to load a magica voxel scene given a filename.
        const ogt_vox_scene *load_vox_scene(const char *filename, uint32_t scene_read_flags = 0) {
            // open the file
#if defined(_MSC_VER) && _MSC_VER >= 1400
            FILE *fp;
            if (0 != fopen_s(&fp, filename, "rb"))
                fp = 0;
#else
            FILE *fp = fopen(filename, "rb");
#endif
            if (!fp)
                return nullptr;

            // get the buffer size which matches the size of the file
            fseek(fp, 0, SEEK_END);
            uint32_t buffer_size = ftell(fp);
            fseek(fp, 0, SEEK_SET);

            // load the file into a memory buffer
            auto *buffer = new uint8_t[buffer_size];
            fread(buffer, buffer_size, 1, fp);
            fclose(fp);

            // construct the scene from the buffer
            const ogt_vox_scene *scene = ogt_vox_read_scene_with_flags(buffer, buffer_size, scene_read_flags);

            // the buffer can be safely deleted once the scene is instantiated.
            delete[] buffer;

            return scene;
        }

        const ogt_vox_scene *load_vox_scene_with_groups(const char *filename) {
            return load_vox_scene(filename, k_read_scene_flags_groups);
        }

        // a helper function to save a magica voxel scene to disk.
        void save_vox_scene(const char *pcFilename, const ogt_vox_scene *scene) {
            // save the scene back out.
            uint32_t buffersize = 0;
            uint8_t *buffer = ogt_vox_write_scene(scene, &buffersize);
            if (!buffer)
                return;

                // open the file for write
#if defined(_MSC_VER) && _MSC_VER >= 1400
            FILE *fp;
            if (0 != fopen_s(&fp, pcFilename, "wb"))
                fp = 0;
#else
            FILE *fp = fopen(pcFilename, "wb");
#endif
            if (!fp) {
                ogt_vox_free(buffer);
                return;
            }

            fwrite(buffer, buffersize, 1, fp);
            fclose(fp);
            ogt_vox_free(buffer);
        }

        // this example just counts the number of solid voxels in this model, but an importer
        // would probably do something like convert the model into a triangle mesh.
        uint32_t count_solid_voxels_in_model(const ogt_vox_model *model) {
            uint32_t solid_voxel_count = 0;
            uint32_t voxel_index = 0;
            for (uint32_t z = 0; z < model->size_z; z++) {
                for (uint32_t y = 0; y < model->size_y; y++) {
                    for (uint32_t x = 0; x < model->size_x; x++, voxel_index++) {
                        // if color index == 0, this voxel is empty, otherwise it is solid.
                        uint32_t color_index = model->voxel_data[voxel_index];
                        bool is_voxel_solid = (color_index != 0);
                        // add to our accumulator
                        solid_voxel_count += (is_voxel_solid ? 1 : 0);
                    }
                }
            }
            return solid_voxel_count;
        }

        void fillHandlerWithCubes(const ogt_vox_model *model, ogt_vox_palette palette, voxie::Chunk *chunk) {
            auto chunkPos = chunk->GetPosition();
            uint32_t voxel_index = 0;
            std::map<uint32_t, int> materials;
            int nrMaterials = 0;
            for (uint32_t z = 0; z < model->size_z; z++) {
                for (uint32_t y = 0; y < model->size_y; y++) {
                    for (uint32_t x = 0; x < model->size_x; x++, voxel_index++) {
                        uint32_t color_index = model->voxel_data[voxel_index];
                        bool is_voxel_solid = (color_index != 0);
                        if (is_voxel_solid) {
                            auto color = palette.color[color_index];
                            auto iterator = materials.find(color_index);
                            if (iterator == materials.end())
                                materials[color_index] = nrMaterials++;
                            float size = 1.0f;
                            auto cube = std::make_unique<Cube>(
                                    Position{{(float) y * size + chunkPos->pos[0] - model->size_y * size / 2,
                                              (float) z * size + chunkPos->pos[1] - model->size_z * size / 2,
                                              (float) x * size + chunkPos->pos[2] - model->size_x * size / 2}},// <---- They use different coordinate system, so here we compensate.
                                    Dimensions{size, size, size},
                                    Material{{(float) color.r / 255.0f, (float) color.g / 255.0f, (float) color.b / 255.0f},
                                             {1.0f, 0.5f, 0.31f},
                                             {0.5f, 0.5f, 0.5f},
                                             32.0f},
                                    materials[color_index]);
                            chunk->AddCube({(int) y, (int) z, (int) x}, std::move(cube));
                        }
                    }
                }
            }
        }

    }// namespace internal

    void LoadModel(const std::string &filename, Chunk *chunk) {
        const ogt_vox_scene *scene = internal::load_vox_scene_with_groups(filename.c_str());
        if (scene) {
            for (uint32_t model_index = 0; model_index < scene->num_models; model_index++) {
                internal::fillHandlerWithCubes(scene->models[model_index], scene->palette, chunk);
            }
            ogt_vox_destroy_scene(scene);
        }
    }
}// namespace voxie::ModelLoader
