
#モジュールのインポート
import bpy
import bpy_extras
import math
import json

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
        #radianで出力
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

        if "type" in object:#カスタムプロパティで指定あり
            json_object["type"] = object["type"]
        else:
            json_object["type"] = object.type

        json_object["name"] = object.name

        #Todo: その他の情報をパック
        #オブジェクトのローカルトランスフォームから平行移動、回転、スケールを抽出
        trans,rot, scale = object.matrix_local.decompose()
        rot = rot.to_euler()
        #radianで出力
        #トランスフォーム情報をディクショナリに登録
        transform = dict()
        transform["translation"] = (trans.x,trans.y,trans.z)
        transform["rotation"] = (rot.x,rot.y,rot.z)
        transform["scaling"] = (scale.x,scale.y,scale.z)
        #まとめて1個分のjsonオブジェクトに登録
        json_object["transform"] = transform

        #カスタムプロパティ'disabled'
        if "disabled" in object:
            json_object["disabled"] = object["disabled"]

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
    