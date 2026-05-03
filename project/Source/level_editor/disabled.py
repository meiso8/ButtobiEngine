import bpy

from .add_disabled import MYADDON_OT_add_disabled

class OBJECT_PT_disabled(bpy.types.Panel):
    """オブジェクトの表示・非表示パネル"""
    bl_idname = "OBJECT_PT_disabled"
    bl_label = "Disabled"
    bl_space_type = "PROPERTIES"
    bl_region_type = "WINDOW"
    bl_context = "object"

    def draw(self,context):
        if "disabled" in context.object:
       #['disabled']カスタムプロパティを追加
            self.layout.prop(context.object,'["disabled"]')
        else:
            self.layout.operator(MYADDON_OT_add_disabled.bl_idname)
 


