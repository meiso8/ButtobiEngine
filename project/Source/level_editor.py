import bpy

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
    
    
def register():
    print("レベルエディタが有効化されました。")

def unregister():
    print("レベルエディタが無効化されました。")
    

if __name__ == "__main__":
    register()