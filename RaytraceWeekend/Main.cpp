#include "pch.h"

#include "Camera.h"
#include "DirectX11/DX11Device.h"
#include "HittableList.h"
#include "Interfaces/CommandList.h"
#include "Material.h"
#include "Math/Color.h"
#include "Math/MathConstants.h"
#include "Math/Ray.h"
#include "Math/Vector3D.h"
#include "Resources/Shaders.h"
#include "Resources/Texture2D.h"
#include "Sphere.h"
#include "Windows/WindowsApplication.h"

extern "C"
{
    // https://docs.nvidia.com/gameworks/content/technologies/desktop/optimus.htm
    __declspec(dllexport) int NvOptimusEnablement = 0x00000001;
    // https://gpuopen.com/amdpowerxpressrequesthighperformance/
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

Color
GammaCorrection(const Color& color)
{
    // clang-format off
    return Color(
        sqrtf(color.R),
        sqrtf(color.G),
        sqrtf(color.B)
    );
    // clang-format on
}

Color
ray_color(const Ray& r, const IHittable& world, int32 depth)
{
    HitRecord rec;

    if (depth <= 0) {
        return Color(0.0f, 0.0f, 0.0f);
    }

    if (world.Hit(r, 0.001f, infinity, rec)) {
        Ray   scattered;
        Color attenuation;
        if (rec.matPtr->Scatter(r, rec, attenuation, scattered)) {
            return attenuation * ray_color(scattered, world, depth - 1);
        }
        return rec.matPtr->Emit();
    }
    Vector3D unit_direction = Normalize(r.Direction);
    float    t              = 0.5f * (unit_direction.Y + 1.0f);
    return 0.025f * Lerp(Color(1.0f, 1.0f, 1.0f), Color(0.5f, 0.7f, 1.0f), t);
}

int32
main(int32 argc, char* argv[])
{
    const uint32 screenWidth  = 1280;
    const uint32 screenHeight = 720;

    std::unique_ptr<IApplication> app;
    app.reset(CreateApplication<WindowsApplication, DX11Device>(L"Test Application", screenWidth, screenHeight));

    app->GetDevice()->SetShaderDir("Shaders/");

    VertexShader* vshader = new VertexShader("FullscreenVS.cso");
    PixelShader*  pshader = new PixelShader("FullscreenPS.cso");

    assert(vshader->IsValid() && pshader->IsValid());

    // Image
    const float aspect_ratio    = 16.0f / 9.0f;
    const int32 image_width     = screenWidth;
    const int32 image_height    = static_cast<int>(image_width / aspect_ratio);
    const int32 samplesPerPixel = 500;
    const int32 maxDepth        = 50;

    Texture2D* output
      = new Texture2D(image_width, image_height, PIXEL_FORMAT_R32G32B32A32, TEXTURE_FLAG_WRITABLE | TEXTURE_FLAG_SHADER_RESOURCE);

    pshader->SetTexture2D(0, output);

    gCommandList.SetVertexShader(vshader->GetGpuShader());
    gCommandList.SetPixelShader(pshader->GetGpuShader());
    gCommandList.SetPrimitiveTopology(PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    gCommandList.SetViewport(0, 0, screenWidth, screenHeight);
    gCommandList.SetScissor(0, 0, screenWidth, screenHeight);

    // World
    // HittableList world = random_scene();
    HittableList world;

    auto material_ground = std::make_shared<LambertMaterial>(Color(0.8f, 0.8f, 0.0f));
    auto material_center = std::make_shared<MetalMaterial>(Color(0.7f, 0.3f, 0.3f), 0.125f);
    auto material_left   = std::make_shared<MetalMaterial>(Color(0.8f, 0.8f, 0.8f), 0.3f);
    auto material_right  = std::make_shared<MetalMaterial>(Color(0.8f, 0.6f, 0.3f), 1.0f);
    auto material_top    = std::make_shared<EmissiveMaterial>(Color(1.0f, 1.0f, 1.0f, 5.0f));

    world.Add(std::make_shared<Sphere>(Vector3D(0.0f, -100.5f, -1.0f), 100.0f, material_ground));
    world.Add(std::make_shared<Sphere>(Vector3D(0.0f, 0.0f, -1.0f), 0.5f, material_center));
    world.Add(std::make_shared<Sphere>(Vector3D(-1.0f, 0.0f, -1.0f), 0.5f, material_left));
    world.Add(std::make_shared<Sphere>(Vector3D(1.0f, 0.0f, -1.0f), 0.5f, material_right));
    world.Add(std::make_shared<Sphere>(Vector3D(0.0f, 1.0f, -.8f), 0.25f, material_top));

    // Camera
    Camera cam;

    std::vector<Color> colorRow(image_width);
    for (int j = image_height - 1; j >= 0; --j) {
        for (int i = 0; i < image_width; ++i) {
            Color pixelColor(0, 0, 0);
            for (int32 s = 0; s < samplesPerPixel; s++) {
                float u = float(i + Random()) / (image_width - 1);
                float v = float(j + Random()) / (image_height - 1);
                Ray   r = cam.GetRay(u, v);
                pixelColor += ray_color(r, world, maxDepth);
            }
            colorRow[i] = GammaCorrection(pixelColor / samplesPerPixel);
        }

        output->SetPixelRow(image_height - j - 1, colorRow.data());

        app->TickFrame();

        gCommandList.SetRenderTargetsDepthStencil({app->GetSurface()->GetBackBuffer()}, nullptr);
        // gCommandList.ClearRT(app->GetSurface()->GetBackBuffer(), {0.0f, 0.0f, 0.0f, 1.0f});
        gCommandList.Draw(3);

        app->GetSurface()->Present();

        if (app->IsExiting()) {
            break;
        }
    }

    while (!app->IsExiting()) {
        app->TickFrame();

        app->GetSurface()->Present();
    }

    SAFE_DELETE(output);

    return 0;
}