import bpy

class MYADDON_OT_add_disabled(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_add_disabled"
    bl_label = "表示切替"
    bl_description = "['disabled']カスタムプロパティを追加します"
    bl_options = {"REGISTER","UNDO"}

    def execute(self, context):
        #['disabled']カスタムプロパティを追加
        context.object["disabled"] =  True;
        return {"FINISHED"}
