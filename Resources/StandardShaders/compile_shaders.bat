echo off
"C:\Program Files (x86)\Windows Kits\8.0\bin\x86\fxc.exe" color_only_vertex.hlsl /Zi  /Qstrip_debug /T vs_4_0 /Ges /O3 /E "main" /Vn "StandardPrograms::color_only_vertex" /Fh "..\..\Sources\D3D\Shaders\color_only_vertex.h"
"C:\Program Files (x86)\Windows Kits\8.0\bin\x86\fxc.exe" color_only_fragment.hlsl /Zi /Qstrip_debug  /T ps_4_0 /Ges /O3 /E "main" /Vn "StandardPrograms::color_only_fragment" /Fh "..\..\Sources\D3D\Shaders\color_only_fragment.h"
"C:\Program Files (x86)\Windows Kits\8.0\bin\x86\fxc.exe" single_texture_vertex.hlsl /Zi /Qstrip_debug /T vs_4_0 /Ges /O3 /E "main" /Vn "StandardPrograms::single_texture_vertex" /Fh "..\..\Sources\D3D\Shaders\single_texture_vertex.h"
"C:\Program Files (x86)\Windows Kits\8.0\bin\x86\fxc.exe" single_texture_fragment.hlsl /Zi /Qstrip_debug /T ps_4_0 /Ges /O3 /E "main" /Vn "StandardPrograms::single_texture_fragment" /Fh "..\..\Sources\D3D\Shaders\single_texture_fragment.h"
"C:\Program Files (x86)\Windows Kits\8.0\bin\x86\fxc.exe" sprite_vertex.hlsl /Zi /Qstrip_debug /T vs_4_0 /Ges /O3 /E "main" /Vn "StandardPrograms::sprite_vertex" /Fh "..\..\Sources\D3D\Shaders\sprite_vertex.h"
"C:\Program Files (x86)\Windows Kits\8.0\bin\x86\fxc.exe" sprite_fragment.hlsl /Zi /Qstrip_debug /T ps_4_0 /Ges /O3 /E "main" /Vn "StandardPrograms::sprite_fragment" /Fh "..\..\Sources\D3D\Shaders\sprite_fragment.h"
"C:\Program Files (x86)\Windows Kits\8.0\bin\x86\fxc.exe" path_vertex.hlsl /Zi /Qstrip_debug /T vs_4_0 /Ges /O3 /E "main" /Vn "StandardPrograms::path_vertex" /Fh "..\..\Sources\D3D\Shaders\path_vertex.h"
"C:\Program Files (x86)\Windows Kits\8.0\bin\x86\fxc.exe" path_fragment.hlsl /Zi /Qstrip_debug /T ps_4_0 /Ges /O3 /E "main" /Vn "StandardPrograms::path_fragment" /Fh "..\..\Sources\D3D\Shaders\path_fragment.h"


pause
