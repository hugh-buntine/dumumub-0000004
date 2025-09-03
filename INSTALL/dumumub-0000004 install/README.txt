# dumumub-0000004 Plugin Installation Guide

## Platform Compatibility

⚠️ **macOS ONLY** - This plugin is only supported on macOS systems. Windows is not supported yet
## Installation Instructions

This package contains both VST3 and Audio Unit (AU) versions of the dumumub-0000004 plugin for macOS.

### VST3 Plugin Installation (macOS)
1. Copy `dumumub-0000004.vst3` to your VST3 plugins folder:
   - **macOS**: `/Library/Audio/Plug-Ins/VST3/` (system-wide) or `~/Library/Audio/Plug-Ins/VST3/` (user-only)
2. Restart your DAW
3. The plugin should appear in your VST3 plugin list

### Audio Unit (AU) Plugin Installation (macOS)
1. Copy `dumumub-0000004.component` to your Audio Units folder:
   - **macOS**: `/Library/Audio/Plug-Ins/Components/` (system-wide) or `~/Library/Audio/Plug-Ins/Components/` (user-only)
2. Restart your DAW
3. The plugin should appear in your Audio Units plugin list

## Important: macOS Security Notice

⚠️ **These plugins are unsigned**, so macOS will initially block them from running.

### To enable the plugins:
1. After copying the plugins, try to use them in your DAW
2. macOS will show a security warning - click "Cancel" or "OK"
3. Go to **System Preferences** → **Security & Privacy** → **General**
4. You should see a message about blocked software with an "Allow Anyway" button
5. Click "**Allow Anyway**" for each plugin (VST3 and AU)
6. Restart your DAW

### Alternative Method (Command Line):
If the above doesn't work, you can remove the quarantine attribute:
```bash
sudo xattr -rd com.apple.quarantine "/Library/Audio/Plug-Ins/VST3/dumumub-0000004.vst3"
sudo xattr -rd com.apple.quarantine "/Library/Audio/Plug-Ins/Components/dumumub-0000004.component"
```

## Troubleshooting

- **Plugin doesn't appear in DAW**: Make sure you've restarted your DAW after installation
- **Still getting security warnings**: Try the command line method above
- **Permission denied**: You may need to use `sudo` when copying to system folders
- **DAW-specific issues**: Some DAWs have their own plugin scanning/validation processes

## Supported DAWs (macOS Only)
This plugin should work with any macOS DAW that supports VST3 or Audio Unit plugins, including:
- Logic Pro
- Pro Tools
- Ableton Live
- Reaper
- GarageBand
- And many others

**Note**: Windows DAWs are not supported.

For support or questions, please refer to the main project documentation.