#define NOMINMAX
#include "Text.h"
#include"DebugUI.h"
#include "FreeTypeManager.h"
#include"Engine/Mesh/Font/Font.h"


void Text::Initialize(uint32_t fontHandle) {
    fontHandle_ = fontHandle;
}

void Text::SetString(const std::u32string& text) {
    text_ = text;
    UpdateLayout();
}

void Text::SetPosition(const Vector2& pos) {
    position_ = pos;
    UpdateLayout();
}

void Text::SetColor(const Vector4& color) {
    color_ = color;
}

void Text::SetAlign(TextAlign align) {
    align_ = align;
    UpdateLayout();
}

void Text::SetBlendMode(const BlendMode& blendMode) {
    blendMode_ = blendMode;
}

void Text::UpdateLayout() {

    glyphRuns_ = FreeTypeManager::LayoutString(fontHandle_, text_, { 0.0f, 0.0f }, Window::GetClientWidth());

    // 中央・右寄せ対応（行ごとにオフセット）
    std::unordered_map<float, float> lineWidths;
    for (const auto& run : glyphRuns_) {
        auto& size = FreeTypeManager::GetGlyphTextures({ fontHandle_, run.glyphIndex }).glyphSize;
        lineWidths[run.position.y] += size.x;
    }

    for (auto& run : glyphRuns_) {
        float offsetX = 0.0f;
        float lineWidth = lineWidths[run.position.y];
        switch (align_) {
        case TextAlign::Center:
            offsetX = -lineWidth * 0.5f;
            break;
        case TextAlign::Right:
            offsetX = -lineWidth;
            break;
        default:
            break;
        }
        run.position.x += position_.x + offsetX;
        run.position.y += position_.y;
    }
}


void Text::Draw() {

    activeFonts_.clear();
    Font::PreDraw(blendMode_);
    for (const auto& run : glyphRuns_) {
        GlyphKey key{ fontHandle_, run.glyphIndex };
        auto* font = FreeTypeManager::GetOrCreateFont(key);
        auto& texData = FreeTypeManager::GetGlyphTextures(key);
        // ベースラインに合わせてY位置を調整！
        float y = run.position.y - (texData.glyphSize.y + texData.bearingY) / 2.0f;
        font->SetPosition({ run.position.x,y });
        font->SetColor(color_);
        font->Update();
        font->Draw();
        activeFonts_.push_back(font);
    }
}

void Text::Debug() {

#ifdef _DEVELOP
    ImGui::Begin("Debug");

    if (ImGui::TreeNode("Font")) {

        DebugUI::CheckColor(color_, "color");
        DebugUI::CheckBlendMode(blendMode_);
        ImGui::SliderFloat2("pos", &position_.x, -1000.0f, 1000.0f);
        UpdateLayout();
        for (size_t i = 0; i < activeFonts_.size(); ++i) {
            auto* font = activeFonts_[i];
            if (font) {
                std::string msg = "Glyph[" + std::to_string(i) + "]";
                DebugUI::CheckFont(*font, msg.c_str());
            }
        }

        ImGui::TreePop();
    }
    ImGui::End();
#endif
}
