#pragma once
#include <PluginProcessor.h>

[[maybe_unused]] static void runWithinPluginEditor (const std::function<void (Dumumub0000004AudioProcessor& plugin)>& testCode)
{
    Dumumub0000004AudioProcessor plugin;
    const auto editor = plugin.createEditorIfNeeded();

    testCode (plugin);

    plugin.editorBeingDeleted (editor);
    delete editor;
}
