### 设置页面

#### 本地音乐路径设置

- 增加扫描路径，使用文件Dialog返回/删除所选path
- 通知core修改
- 更新listWidget页面显示内容


#### 活动流程

- Widget添加了路径path
- 发送信号signalAddButton(path)
- Core接收信号，执行addLocalMusicPath(path)
- 向本地json文件添加项，并发送settings已经被修改的通知signalLocalSettingsChanged();
- signalLocalSettingsChanged()信号会被发送回ui层，调用SettingsWidget::updateLocalPaths()方法进行更新
