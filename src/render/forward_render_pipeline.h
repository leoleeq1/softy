#ifndef RENDER_FORWARD_RENDER_PIPELINE_H_
#define RENDER_FORWARD_RENDER_PIPELINE_H_

#include "render/camera.h"
#include "render/render_pipeline.h"

namespace softy {
class ForwardRenderPipeline : public RenderPipeline {
 public:
  ForwardRenderPipeline() = default;
  virtual ~ForwardRenderPipeline() = default;

  virtual void Render(Camera* camera) override;
};
}  // namespace softy

#endif  // RENDER_FORWARD_RENDER_PIPELINE_H_