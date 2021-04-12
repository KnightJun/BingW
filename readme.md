<div align=center>
    <img width="120" height="120" src="./src/main.ico">
</div>  

English | [简体中文](./readme_zh-cn.md)  

# BingW  
Automatically set wallpaper as bing daily wallpaper  

## Contents

- [Background](#Background)
- [Installation](#Installation)
- [Usage](#Usage)
    - [Features](#Features)
- [Dependency](#Dependency)
- [Compile](#Compile)
- [License](#License)

## Background  
I wrote this program because the official bing wallpaper program didn't work on my windows 10 and there were no error messages, so I write this using Qt and added a search function via Google earth.  

## Installation  
After downloading the zip archive from release, extract it to the specified directory.

## Usage  
![avatar](./Images/usage.webp)  
Open BingW.exe, it can automatically detect and set the latest bing wallpaper, after starting the program will be displayed in the taskbar tray area, right-click the icon can see some function options, the default is to start with the system, you can right-click through the tray to close.  

### Features  
* Toggle recent week's wallpaper.  
* Search information of the wallpaper by Bing.  
* Search address by Google earth(Only for landscape wallpapers).  
* Keep current wallpaper, no automatic updates.  

## Dependency
* [Qt](https://www.qt.io/) A prowerfull and cross-paltform C++ framework.  
* [xmake](https://github.com/xmake-io/xmake) A easy to use build system, base on Lua.  

## Compile
At present, only windows system is supported, it is recommended to use MSVB to compile. After installing Qt and xmake, excute `xmake` command in the source directoy to compile.

## License
[Apache License 2.0](https://www.apache.org/licenses/LICENSE-2.0)

---
<div align="center">
<h3>For rabbit</h3>
</div>
