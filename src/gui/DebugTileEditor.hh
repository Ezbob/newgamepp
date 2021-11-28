#pragma once

class DebugTileEditor {

public:

    DebugTileEditor();

    virtual ~DebugTileEditor() = default;
    DebugTileEditor(DebugTileEditor const &) = default;
    constexpr DebugTileEditor(DebugTileEditor &&) = default;
    DebugTileEditor &operator=(DebugTileEditor const &) = default;
    constexpr DebugTileEditor &operator=(DebugTileEditor &&) = default;    

    bool render();
};
