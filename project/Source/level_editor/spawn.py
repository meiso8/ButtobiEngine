import bpy
import os
import bpy.ops
import copy

class SpawnNames():
    #インデックス
    PROTOTYPE = 0 #プロトタイプのオブジェクト名
    INSTANCE  = 1 #量産時のオブジェクト名
    FILENAME = 2 #リソースファイル名

    names = {}
    #names["キー"] =　(プロトタイプのオブジェクト名、量産時のオブジェクト名、リソースファイル名)
    names["Enemy"] = ("PrototypeEnemySpawn","EnemySpawn","coffin/coffin.gltf")
    names["Player"] = ("PrototypePlayerSpawn","PlayerSpawn","player/player.obj") 

#オペレータ　出現ポイントのシンボルを読み込む
class MYADDON_OT_spawn_import_symbol(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_spawn_import_symbol"
    bl_label = "出現ポイントシンボルImport"
    bl_description = "出現ポイントのシンボルをImportします"

    def load_obj(self,self_type):
        print("出現ポイントのシンボルをImportします")
        #重複modelロード防止
        spawn_object = bpy.data.objects.get(SpawnNames.names[self_type][SpawnNames.PROTOTYPE])
        if spawn_object is not None:
            return{'CANCELLED'}

        #スクリプトが配置されているディレクトリの名前を取得する
        addon_directory = os.path.dirname(__file__)
        #ディレクトリからのモデルファイルの相対パスを記述
        relative_path = SpawnNames.names[self_type][SpawnNames.FILENAME]
        #合成してモデルファイルのフルパスを得る
        full_path = os.path.join(addon_directory,relative_path)
        #オブジェクトをインポートEXEC_DEFAULTは他のオペレータを呼び出すとき
        # invole()ではなくexecute()を実行させるためのオプションである

        objs_before = set(bpy.data.objects)
        colls_before = set(bpy.data.collections)


        #拡張子ごとに処理を分ける
        if full_path.endswith('.gltf') or full_path.endswith('.glb'):
            bpy.ops.import_scene.gltf(filepath=full_path)
        elif full_path.endswith('.obj'):
            bpy.ops.wm.obj_import('EXEC_DEFAULT',
            filepath=full_path,display_type='THUMBNAIL',
            forward_axis='Z',up_axis="Y")
        
        #回転を適用
        bpy.ops.object.transform_apply(location=False,
            rotation=True, scale=False, properties=False,
            isolate_users=False)
        
        #アクティブなオブジェクトを取得
        new_objs =  set(bpy.data.objects)-objs_before
        new_colls = set(bpy.data.collections)-colls_before
        
        root_object = bpy.context.active_object

        if root_object:
            #オブジェクト名を変更
            root_object.name = SpawnNames.names[self_type][SpawnNames.PROTOTYPE]
            #オブジェクトの種類を設定
            root_object["type"] =  SpawnNames.names[self_type][SpawnNames.INSTANCE]

        #メモリ上には置いておくがシーン配置から外す
       # インポートされた【すべて】のオブジェクトをリンクを解除
        for obj in new_objs:
            for coll in obj.users_collection:
                coll.objects.unlink(obj)

        # glTF_not_exportedなどの「余分なコレクション」が作られていたら削除
        for coll in new_colls:
            bpy.data.collections.remove(coll)
        
        return {"FINISHED"}
    
    def execute(self,context):
        #Enemyオブジェクト読み込み
        self.load_obj("Enemy")
        #Playerオブジェクト読み込み
        self.load_obj("Player")

        return {'FINISHED'}
    
#オペレータ　出現ポイントのシンボルを作成・配置する
class MYADDON_OT_spawn_create_puton_symbol(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_spawn_create_puton_symbol"
    bl_label = "出現ポイントシンボルの作成"
    bl_description = "出現ポイントのシンボルを作成します"
    bl_options = {'REGISTER','UNDO'}

    self_type : bpy.props.StringProperty(name="Type",default="Player")

    def copy_hierarchy(self,obj, target_collection):
            #複製元の非表示オブジェクトを複製する
            new_obj = obj.copy()
            #複製したオブジェクトを現在のシーンにリンク(出現させる)
            target_collection.objects.link(new_obj)

            # .3子供がいる場合
            for child in obj.children:
                new_child = self.copy_hierarchy(child,target_collection)
                new_child.parent = new_obj
                new_child.name =  SpawnNames.names[self.self_type][SpawnNames.INSTANCE]
                for mod in new_child.modifiers:
                    if mod.type == 'ARMATURE' and mod.object == obj:
                        mod.object = new_obj

            return new_obj 

    def execute(self,context):
        #読み込み済みのコピー元オブジェクトを検索
        spawn_object = bpy.data.objects.get(SpawnNames.names[self.self_type][SpawnNames.PROTOTYPE])

        #未読み込みの場合
        if spawn_object is None:
        #読み込みオペレータを実行
            bpy.ops.myaddon.myaddon_ot_spawn_import_symbol('EXEC_DEFAULT')
            #再検索。今度は見つかるはず
            spawn_object = bpy.data.objects.get(SpawnNames.names[self.self_type][SpawnNames.PROTOTYPE])
    
        print("出現ポイントのシンボルを作成します")

        #Blenderでの選択を解除する
        bpy.ops.object.select_all(action = 'DESELECT')

        new_root_object = self.copy_hierarchy(spawn_object,  bpy.context.collection)
        #オブジェクト名を変更
        new_root_object.name = SpawnNames.names[self.self_type][SpawnNames.INSTANCE]

        return {'FINISHED'}

#自キャラ専用出現ポイントシンボル作成オペレータ
class  MYADDON_OT_spawn_player_create_puton_symbol(bpy.types.Operator):

    bl_idname = "myaddon.myaddon_ot_spawn_player_create_puton_symbol"
    bl_label = "プレイヤー出現ポイントシンボルの作成"
    bl_description = "プレイヤー出現ポイントのシンボルを作成します"

    def execute(self,context):
        bpy.ops.myaddon.myaddon_ot_spawn_create_puton_symbol('EXEC_DEFAULT',self_type= "Player")
        return {'FINISHED'}
    
    #敵キャラ専用出現ポイントシンボル作成オペレータ
class  MYADDON_OT_spawn_enemy_create_puton_symbol(bpy.types.Operator):

    bl_idname = "myaddon.myaddon_ot_spawn_enemy_create_puton_symbol"
    bl_label = "敵出現ポイントシンボルの作成"
    bl_description = "敵出現ポイントのシンボルを作成します"

    def execute(self,context):
        bpy.ops.myaddon.myaddon_ot_spawn_create_puton_symbol('EXEC_DEFAULT',self_type= "Enemy")
        return {'FINISHED'}