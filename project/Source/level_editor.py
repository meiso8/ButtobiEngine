import bpy
import math
import bpy_extras
import gpu
#シェーダ、トポロジー、頂点バッファ、インデックスをまとめたジオメトリバッチという描画に使用するモジュール
import gpu_extras.batch
import copy
#数学のモジュール
import mathutils
import json

bl_info = {
    "name": "レベルエディタ",
    "author" :"Taro Kamata",
    "version": (1,0),
    "blender": (3,3,1),
    "location" :"",
    "descriptioin": "レベルエディタ",
    "warning": "",
    "wiki_url" : "",
    "tracker_url": "",
    "category": "Object"
}
    
#メニュー項目描画
def draw_menu_manual(self,context):
    self.layout.operator("wm.url_open_preset",text ="Manual",icon ="HELP")

class MYADDON_OT_add_collider(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_add_collider"
    bl_label = "コライダー 追加"
    bl_description = "['collider']カスタムプロパティを追加します"
    bl_options = {"REGISTER","UNDO"}


    def execute(self, context):
        #['collider']カスタムプロパティを追加
        context.object["collider"] = "BOX"
        context.object["collider_center"] = mathutils.Vector((0,0,0))
        context.object["collider_size"] = mathutils.Vector((2,2,2))

        return {"FINISHED"}
    
#パネル　コライダー
class OBJECT_PT_collider(bpy.types.Panel):
    bl_idname = "OBJECT_PT_collider"
    bl_label = "Collider"
    bl_space_type = "PROPERTIES"
    bl_region_type = "WINDOW"
    bl_context = "object"

    def draw(self,context):
        if "collider" in context.object:
            self.layout.prop(context.object,'["collider"]',text = "Type")
            self.layout.prop(context.object,'["collider_center"]',text = "Center")
            self.layout.prop(context.object,'["collider_size"]',text = "Size")
        else:
            self.layout.operator(MYADDON_OT_add_collider.bl_idname)



class DrawCollider:
    #描画ハンドル
    handle = None
    #3Dビューに登録する描画関数
    def draw_collider():
        #頂点データ動的配列に変更
        vertices = {"pos":[]}
        #インデックスデータ動的配列に変更
        indices = []
        #各頂点の、オブジェクト中心からのオフセット
        offsets = [
                    [-0.5,-0.5,-0.5],
                    [+0.5,-0.5,-0.5],
                    [-0.5,+0.5,-0.5],
                    [+0.5,+0.5,-0.5],
                    [-0.5,-0.5,+0.5],
                    [+0.5,-0.5,+0.5],
                    [-0.5,+0.5,+0.5],
                    [+0.5,+0.5,+0.5],
        ]

        #立方体のX,Y,Z方向のサイズ
        size = [2,2,2]

        #走査
        for object in bpy.context.scene.objects:
          
            #コライダープロパティが無ければ、描画をスキップ
            if not "collider" in object:
                continue
            
            center = mathutils.Vector((0,0,0))
            size = mathutils.Vector((2,2,2))

            #プロパティから値を取得する
            center[0]=object["collider_center"][0]
            center[1]=object["collider_center"][1]
            center[2]=object["collider_center"][2]
            size[0]=object["collider_size"][0]
            size[1]=object["collider_size"][1]
            size[2]=object["collider_size"][2]

            #追加前の頂点数
            start = len(vertices["pos"])

            for offset in offsets:
                #ローカル位置をコピーする
                pos = copy.copy(center)
                pos[0]+=offset[0]*size[0]
                pos[1]+=offset[1]*size[1]
                pos[2]+=offset[2]*size[2]
                #ローカルからワールド座標に変換 @が乗算
                pos = object.matrix_world @ pos
                vertices['pos'].append(pos)

                #前面   
                indices.append([start+0,start+1])
                indices.append([start+2,start+3])
                indices.append([start+0,start+2])
                indices.append([start+1,start+3])
                #奥面
                indices.append([start+4,start+5])
                indices.append([start+6,start+7])
                indices.append([start+4,start+6])
                indices.append([start+5,start+7])
                #手前遠くをつなぐ辺
                indices.append([start+0,start+4])
                indices.append([start+1,start+5])
                indices.append([start+2,start+6])
                indices.append([start+3,start+7])              

        #シェーダーを取得
        shader = gpu.shader.from_builtin("UNIFORM_COLOR")
        #バッチ作成
        batch = gpu_extras.batch.batch_for_shader(shader, "LINES" , vertices, indices = indices)
        #シェーダーのパラメータ
        color = [0.5, 1.0, 1.0 ,1.0]
        shader.bind()
        shader.uniform_float("color", color)
        #描画
        batch.draw(shader)

class MYADDON_OT_stretch_vertex(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_stretch_vertex"
    bl_label = "頂点を伸ばす"
    bl_description = "頂点座標を引っ張って伸ばします"
    bl_options = {'REGISTER','UNDO'}

    def execute(self,context):
        bpy.data.objects["Cube"].data.vertices[0].co.x += 1.0
        print("頂点を伸ばしました。")
        return {'FINISHED'}
 

class MYADDON_OT_create_ico_sphere(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_create_object"
    bl_label = "ICO級生成"
    bl_description = "ICO球を生成します"
    bl_options = {'REGISTER','UNDO'}
    
    def execute(self,context):
        bpy.ops.mesh.primitive_ico_sphere_add()
        print("ICO球を生成しました。")
        return {'FINISHED'}
    
#多重継承　オペレータかつエクスポートヘルパー
class MYADDON_OT_export_scene(bpy.types.Operator,bpy_extras.io_utils.ExportHelper):
    bl_idname = "myaddon.myaddon_ot_export_scene"
    bl_label = "シーン出力"
    bl_description = "シーン情報をExportします"

    #出力するファイルの拡張子
    filename_ext = ".json"

    def write_and_print(self,file,str):
        print(str)
        file.write(str)
        file.write('\n')


    def parse_scene_recursive(self,file,object,level):
        """シーン解析用再帰関数"""

        indent = ''
        for i in range(level):
            indent += "\t"

        self.write_and_print(file,indent + object.type)
        trans,rot, scale = object.matrix_local.decompose()
        rot = rot.to_euler()
        rot.x = math.degrees(rot.x)
        rot.y = math.degrees(rot.y)
        rot.z = math.degrees(rot.z)
        self.write_and_print(file,indent + "T %f,%f,%f" % (trans.x,trans.y,trans.z))
        self.write_and_print(file,indent + "R %f,%f,%f"% (rot.x,rot.y,rot.z))
        self.write_and_print(file,indent + "S %f,%f,%f"% (scale.x,scale.y,scale.z))
        #カスタムプロパティ'file_name'
        if "file_name" in object:
            self.write_and_print(file, indent + "N %s" % object["file_name"])
        #カスタムプロパティ'collision'
        if "collider" in object:
            self.write_and_print(file, indent + "C %s" % object["collider"])
            self.write_and_print(file,indent + "CC %f %f %f" % ( object["collider_center"][0],object["collider_center"][1],object["collider_center"][2]))
            self.write_and_print(file,indent + "CS %f %f %f" % ( object["collider_size"][0],object["collider_size"][1],object["collider_size"][2]))

        #カスタムプロパティ'disabled'
        if "disabled" in object:
            self.write_and_print(file, indent + "D %s" % object["disabled"])

        self.write_and_print(file, indent + 'END')
        self.write_and_print(file,'')
        
        #子ノードに進む
        for child in object.children:
            self.parse_scene_recursive(file,child,level + 1)

    #親となるlist,処理対象のBlenderオブジェクト,再帰呼び出しの深さレベルを引数とする
    def parse_scene_recursive_json(self,data_parent,object,level):

        #シーンのオブジェクト1個分のjsonオブジェクト生成
        json_object = dict();
        json_object["type"] = object.type
        json_object["name"] = object.name

        #Todo: その他の情報をパック
        #オブジェクトのローカルトランスフォームから平行移動、回転、スケールを抽出
        trans,rot, scale = object.matrix_local.decompose()
        rot = rot.to_euler()
        rot.x = math.degrees(rot.x)
        rot.y = math.degrees(rot.y)
        rot.z = math.degrees(rot.z)
        #トランスフォーム情報をディクショナリに登録
        transform = dict()
        transform["translation"] = (trans.x,trans.y,trans.z)
        transform["rotation"] = (rot.x,rot.y,rot.z)
        transform["scaling"] = (scale.x,scale.y,scale.z)
        #まとめて1個分のjsonオブジェクトに登録
        json_object["transform"] = transform

        #カスタムプロパティ'file_name'
        if "file_name" in object:
            json_object["file_name"] = object["file_name"]

        #カスタムプロパティ'collider'
        if "collider" in object:
            collider = dict()
            collider["type"] = object["collider"]
            #json用に変換するto_list
            collider["center"] = object["collider_center"].to_list()
            collider["size"] = object["collider_size"].to_list()
            json_object["collider"] = collider

            #カスタムプロパティ'disabled'
        if "disabled" in object:
            json_object["disabled"] = object["disabled"]

        #1個分のjsonオブジェクトを親オブジェクトに登録　
        data_parent.append(json_object)
        #Todo:直接の子供リストを走査
        if len(object.children) > 0:
            #子ノードリストを作成
            json_object["children"] = list()
            #子ノードへ進む(深さが1上がる)
            for child in object.children:
                self.parse_scene_recursive_json(json_object["children"],child,level + 1 )


    def export_json(self):
        """JSON形式でファイルに出力"""
        #保存する情報をまとめるdict
        json_object_root = dict()
        #ノード名
        json_object_root["name"] = "scene"
        #オブジェクトリストを作成
        json_object_root["objects"] = list()

        #Todo:シーン内の全オブジェクト走査をしてパック
        for object in bpy.context.scene.objects:
            #親オブジェクトがあるものはスキップ(代わりに親から呼び出すため)
            if(object.parent):
                continue
            #シーン直下のオブジェクトをルートノード(深さ0)とし、再起関数で走査
            self.parse_scene_recursive_json(json_object_root["objects"],object,0)

        #オブジェクトをJSON文字列にエンコード アスキーにするか、エンコーダの設定、インデント幅
        json_text = json.dumps(json_object_root, ensure_ascii=False,indent=4)
        #コンソール表示
        print(json_text)
        #ライルをテキスト形式で書き出し用にオープン
        #スコープを抜けると自動的にクローズされる
        with open(self.filepath, "wt" ,encoding="utf-8")as file:
            #ファイルに文字列を書き込む
            file.write(json_text)



    def export(self):
        """ファイルに出力"""
        print("シーン情報出力開始... %r" % self.filepath);

        with open(self.filepath, "wt") as file:
            self.write_and_print(file,"SCENE")

            for object in bpy.context.scene.objects:
                     
                if (object.parent):
                    continue

                self.parse_scene_recursive(file,object,0);

    
    def execute(self,context):
        print("シーン情報をExportします")      
        self.export_json()
        self.report({'INFO'},"シーン情報をExportしました")
        print("シーン情報をExportしました")
        return {'FINISHED'}
    
class MYADDON_OT_add_disabled(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_add_disabled"
    bl_label = "表示切替"
    bl_description = "['disabled']カスタムプロパティを追加します"
    bl_options = {"REGISTER","UNDO"}

    def execute(self, context):
        #['disabled']カスタムプロパティを追加
        context.object["disabled"] =  False;
        return {"FINISHED"}

class OBJECT_PT_disabled(bpy.types.Panel):
    """オブジェクトの表示・非表示パネル"""
    bl_idname = "OBJECT_PT_disabled"
    bl_label = "Disabled"
    bl_space_type = "PROPERTIES"
    bl_region_type = "WINDOW"
    bl_context = "object"

    def draw(self,context):
        if "disabled" in context.object:
            self.layout.prop(context.object,'["disabled"]',text=self.bl_label)
        else:
            self.layout.operator(MYADDON_OT_add_disabled.bl_idname)
 

class MYADDON_OT_add_filename(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_add_filename"
    bl_label = "FileName 追加"
    bl_description = "['file_name']カスタムプロパティを追加します"
    bl_options = {"REGISTER","UNDO"}

    def execute(self,context):  
        context.object["file_name"] = ""
        return {"FINISHED"}

class OBJECT_PT_file_name(bpy.types.Panel):
    """オブジェクトのファイルネームパネル"""
    bl_idname = "OBJECT_PT_file_name"
    bl_label = "FileName"
    bl_space_type = "PROPERTIES"
    bl_region_type = "WINDOW"
    bl_context = "object"

    # サブメニューの描画
    def draw(self,context):
        #パネルに項目を追加
        if "file_name" in context.object:
            #既にプロパティがあれば、プロパティを表示
            self.layout.prop(context.object,'["file_name"]',text=self.bl_label)
        else:
            self.layout.operator(MYADDON_OT_add_filename.bl_idname)




class TOPBAR_MT_my_menu(bpy.types.Menu):
    bl_idname = "TOPBAR_MT_my_menu"
    bl_label = "MyMenu"
    bl_description = "拡張メニュー by " + bl_info["author"]

    def draw(self,context):
        self.layout.operator(MYADDON_OT_stretch_vertex.bl_idname,text = MYADDON_OT_stretch_vertex.bl_label)
        self.layout.separator()
        self.layout.operator(MYADDON_OT_create_ico_sphere.bl_idname,text = MYADDON_OT_create_ico_sphere.bl_label)
        self.layout.separator()
        self.layout.operator(MYADDON_OT_export_scene.bl_idname,text = MYADDON_OT_export_scene.bl_label)
        self.layout.separator()
        self.layout.operator("wm.url_open_preset",text ="Manual",icon ="HELP")


    def submenu(self,context):
        self.layout.menu(TOPBAR_MT_my_menu.bl_idname)

classes = (
    MYADDON_OT_stretch_vertex,
    MYADDON_OT_create_ico_sphere,
    MYADDON_OT_export_scene,
    TOPBAR_MT_my_menu,
    MYADDON_OT_add_filename,
    OBJECT_PT_file_name,
    MYADDON_OT_add_collider,
    OBJECT_PT_collider,
    MYADDON_OT_add_disabled,
  OBJECT_PT_disabled,
           )


    
def register():
    for cls in classes:
        bpy.utils.register_class(cls)
    #メニューに項目を追加
    bpy.types.TOPBAR_MT_editor_menus.append(TOPBAR_MT_my_menu.submenu)
    #3Dビューに描画関数を追加
    DrawCollider.handle = bpy.types.SpaceView3D.draw_handler_add(DrawCollider.draw_collider, (), "WINDOW","POST_VIEW")
    print("レベルエディタが有効化されました。")

def unregister():
    bpy.types.TOPBAR_MT_editor_menus.remove(TOPBAR_MT_my_menu.submenu)
    #3Dビューから描画関数を削除
    bpy.types.SpaceView3D.draw_handler_remove(DrawCollider.handle,"WINDOW")
    #Blenderからクラスを削除
    for cls in classes:
        bpy.utils.unregister_class(cls)

    print("レベルエディタが無効化されました。")

if __name__ == "__main__":
    register()