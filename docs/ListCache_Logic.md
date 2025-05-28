### Cache的逻辑

#### 提供的api
- 接受`QString`返回对应`QVector<Song>`

#### init
- 从`settings`类解析的localMusicPath初始化本地列表
- 依赖`settings`类解析的User keys, 从Database初始化用户自定义列表
- 从`Settings`读取需要初始化的User列表名称，再根据名称一一从数据库读取`QVector<Song>`插入Hash表


#### 刷新机制（部分情况考虑增加自动更新逻辑，每个列表ui增加手动刷新交互）
- 提供刷新本地列表的方法，当本地音乐扫描文件夹增加/删除时就更新一次
- 提供更新用户歌单的方法，当用户歌单被删除，创建时新增Hash