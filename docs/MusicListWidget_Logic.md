## 播放列表组件
完成用户切换播放列表的需求，并且可以让用户新增播放列表，删除播放列表，其结果跟随写入数据库中
### 绘制内容
- Local Button 本地音乐按钮
- Expand Button & Add Button 展开和隐藏，增加歌单
- User Button Area 可伸缩的用户自定义歌单区域

### 接收的信号
- Core发来的settings.json发生了修改，会更新一次界面，也就是有歌单被删除或者添加

### 发送的信号

- 切换歌单，发送一个QString到Core，用于Core进行查询
- 添加歌单，首先写入settings.json，settings.json中会记录全部的用户歌单的名称
- 删除歌单，通知Core一个QString，Core负责调用删除函数来删除settings中的歌单名称，但是数据中的数据表并不会删除
- 展开和收起歌单