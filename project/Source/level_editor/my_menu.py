import bpy

from .stretch_vertex import MYADDON_OT_stretch_vertex
from .create_ico_sphere import MYADDON_OT_create_ico_sphere
from .export_scene import MYADDON_OT_export_scene
from .spawn import MYADDON_OT_spawn_import_symbol
from .spawn import MYADDON_OT_spawn_create_puton_symbol

class TOPBAR_MT_my_menu(bpy.types.Menu):
        
    bl_idname = "TOPBAR_MT_my_menu"
    bl_label = "MyMenu"
    bl_description = "拡張メニュー by meiso"

    def draw(self,context):
        #頂点ストレッチ
        self.layout.operator(MYADDON_OT_stretch_vertex.bl_idname,text = MYADDON_OT_stretch_vertex.bl_label)
        self.layout.separator()
        #ICO球の作成
        self.layout.operator(MYADDON_OT_create_ico_sphere.bl_idname,text = MYADDON_OT_create_ico_sphere.bl_label)
        self.layout.separator()
        #シーンの出力
        self.layout.operator(MYADDON_OT_export_scene.bl_idname,text = MYADDON_OT_export_scene.bl_label)
        self.layout.separator()
        #シンボルのインポート
        self.layout.operator(MYADDON_OT_spawn_import_symbol.bl_idname,text = MYADDON_OT_spawn_import_symbol.bl_label)
        self.layout.separator()
        #シンボル作成
        self.layout.operator(MYADDON_OT_spawn_create_puton_symbol.bl_idname,text = MYADDON_OT_spawn_create_puton_symbol.bl_label)
        self.layout.separator()
        #メニュー項目描画
        self.layout.operator("wm.url_open_preset",text ="Manual",icon ="HELP")


    def submenu(self,context):
        self.layout.menu(TOPBAR_MT_my_menu.bl_idname)