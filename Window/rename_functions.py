#!/usr/bin/python3

import os
import re

# PascalCase → camelCase conversion table
conversion_map = {
    # Public
    "Close": "close",
    "GetPosition": "getPosition",
    "GetSize": "getSize",
    "GetKeyState": "getKeyState",
    "GetBtnState": "getBtnState",
    "GetMousePos": "getMousePos",
    "GetGamepad": "getGamepad",
    "IsRunning": "isRunning",
    "Width": "width",
    "Height": "height",
    "Resized": "resized",
    "GetScale": "getScale",
    "SetScale": "setScale",
    "GetDisplayScale": "getDisplayScale",
    "IsFullscreen": "isFullscreen",
    "GetClipboardText": "getClipboardText",
    "SetClipboardText": "setClipboardText",
    "ShowKeyboard": "showKeyboard",
    "SetTitle": "setTitle",
    "SetPosition": "setPosition",
    "SetSize": "setSize",
    "GetNativeHandle": "getNativeHandle",
    "ShowImage": "showImage",
    "SetCursor": "setCursor",
    "SetFullscreen": "setFullscreen",
    "SetSizeScaled": "setSizeScaled",
    "GetEvent": "getEvent",
    "ProcessEvents": "processEvents",
    "ProcessEvent": "processEvent",
    "PollEvents": "pollEvents",
    "WaitEvents": "waitEvents",

    # Protected
    "MouseEvent": "mouseEvent",
    "KeyEvent": "keyEvent",
    "TextEvent": "textEvent",
    "MoveEvent": "moveEvent",
    "ResizeEvent": "resizeEvent",
    "FocusEvent": "focusEvent",
    "GPadConnect": "gpadConnect",
    "GPadButton": "gpadButton",
    "GPadAxis": "gpadAxis",
    "CloseEvent": "closeEvent",

    # Event handlers
    "OnMouseEvent": "onMouseEvent",
    "OnKeyEvent": "onKeyEvent",
    "OnTextEvent": "onTextEvent",
    "OnMoveEvent": "onMoveEvent",
    "OnResizeEvent": "onResizeEvent",
    "OnFocusEvent": "onFocusEvent",
    "OnTouchEvent": "onTouchEvent",
    "OnGPadConnect": "onGpadConnect",
    "OnGPadButton": "onGpadButton",
    "OnGPadAxis": "onGpadAxis",
    "OnCloseEvent": "onCloseEvent"
}

# File extensions to process
extensions = [".h", ".hpp", ".cpp", ".cc"]

def rename_in_file(filepath):
    with open(filepath, "r", encoding="utf-8") as file:
        content = file.read()

    original_content = content

    for old, new in conversion_map.items():
        # Replace function names with word boundaries
        content = re.sub(rf"\b{old}\b", new, content)

    if content != original_content:
        with open(filepath, "w", encoding="utf-8") as file:
            file.write(content)
        print(f"Updated: {filepath}")

def process_directory(root_dir):
    for root, _, files in os.walk(root_dir):
        for filename in files:
            if any(filename.endswith(ext) for ext in extensions):
                filepath = os.path.join(root, filename)
                rename_in_file(filepath)

if __name__ == "__main__":
    #src_dir = input("Enter the path to your source directory: ").strip()
    #process_directory(src_dir)
    process_directory('.')
    print("✅ All functions renamed successfully!")
    

