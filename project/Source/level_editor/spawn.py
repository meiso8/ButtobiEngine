import bpy
import os
import bpy.ops
import copy

#オペレータ　出現ポイントのシンボルを読み込む
class MYADDON_OT_spawn_import_symbol(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_spawn_import_symbol"
    bl_label = "出現ポイントシンボルImport"
    bl_description = "出現ポイントのシンボルをImportします"

    prototype_object_name = "PrototypePlayerSpawn"
    object_name = "PlayerSpawn"

    def execute(self,context):
        print("出現ポイントのシンボルをImportします")

        #重複modelロード防止
        spawn_object = bpy.data.objects.get(MYADDON_OT_spawn_import_symbol.prototype_object_name)
        if spawn_object is not None:
            return{'CANCELLED'}

        #スクリプトが配置されているディレクトリの名前を取得する
        addon_directory = os.path.dirname(__file__)
        #ディレクトリからのモデルファイルの相対パスを記述
        relative_path = "player/player.obj"
        #合成してモデルファイルのフルパスを得る
        full_path = os.path.join(addon_directory,relative_path)
        #オブジェクトをインポートEXEC_DEFAULTは他のオペレータを呼び出すとき
        # invole()ではなくexecute()を実行させるためのオプションである
        bpy.ops.wm.obj_import('EXEC_DEFAULT',
            filepath=full_path,display_type='THUMBNAIL',
            forward_axis='Z',up_axis="Y")
        #回転を適用
        bpy.ops.object.transform_apply(location=False,
            rotation=True, scale=False, properties=False,
            isolate_users=False)
        #アクティブなオブジェクトを取得
        object = bpy.context.active_object
        #オブジェクト名を変更
        object.name = MYADDON_OT_spawn_import_symbol.prototype_object_name
        #オブジェクトの種類を設定
        object["type"] = MYADDON_OT_spawn_import_symbol.object_name
        #メモリ上には置いておくがシーン配置から外す
        bpy.context.collection.objects.unlink(object)

        return {"FINISHED"}
    
#オペレータ　出現ポイントのシンボルを作成・配置する
class MYADDON_OT_spawn_create_puton_symbol(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_spawn_create_puton_symbol"
    bl_label = "出現ポイントシンボルの作成"
    bl_description = "出現ポイントのシンボルを作成します"
    bl_options = {'REGISTER','UNDO'}

    def execute(self,context):
        #読み込み済みのコピー元オブジェクトを検索
        spawn_object = bpy.data.objects.get(MYADDON_OT_spawn_import_symbol.prototype_object_name)

        #未読み込みの場合
        if spawn_object is None:
        #読み込みオペレータを実行
            bpy.ops.myaddon.myaddon_ot_spawn_import_symbol('EXEC_DEFAULT')
            #再検索。今度は見つかるはず
            spawn_object = bpy.data.objects.get(MYADDON_OT_spawn_import_symbol.prototype_object_name)
    
        print("出現ポイントのシンボルを作成します")

        #Blenderでの選択を解除する
        bpy.ops.object.select_all(action = 'DESELECT')
        #複製元の非表示オブジェクトを複製する
        object = spawn_object.copy()
        #複製したオブジェクトを現在のシーンにリンク(出現させる)
        bpy.context.collection.objects.link(object)
        #オブジェクト名を変更
        object.name = MYADDON_OT_spawn_import_symbol.object_name

        return {'FINISHED'}

  
      

