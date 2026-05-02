#モジュールのインポート
import bpy
#数学のモジュール
import mathutils

import gpu
#シェーダ、トポロジー、頂点バッファ、インデックスをまとめたジオメトリバッチという描画に使用するモジュール
import gpu_extras.batch
import copy

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



