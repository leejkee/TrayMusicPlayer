### 播放列表模块

#### 提供的接口

- 加载歌单内容，一般直接从ListCache提供的函数读写
- 提供获取当前音乐信息的函数
  - 当前音乐文件的绝对路径
  - 当前音乐在列表中的index
- 列表循环的控制
  - 上一曲
  - 下一曲
- 修改播放模式，由ui控制，依次更改播放模式


#### 信号
- signalMusicChanged 同时发送当前音乐文件的index, title, duration
- signalPlayModeChanged 发送代表PlayMode的枚举值