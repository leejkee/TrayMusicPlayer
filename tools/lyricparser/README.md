### Lyric Parser
#### Style supported
- Parse line by line
```lyc
[mm:ss.xx] 第一行歌词
[mm:ss.xx] 第二行歌词
...
[mm:ss.xx] 最后一行歌词
```
- Parse word by word

```lrc
[mm:ss.xx] <mm:ss.xx> 第一行第一个词 <mm:ss.xx> 第一行第二个词 <mm:ss.xx> ... 第一行最后一个词 <mm:ss.xx>
[mm:ss.xx] <mm:ss.xx> 第二行第一个词 <mm:ss.xx> 第二行第二个词 <mm:ss.xx> ... 第二行最后一个词 <mm:ss.xx>
...
[mm:ss.xx] <mm:ss.xx> 最后一行第一个词 <mm:ss.xx> 最后一行第二个词 <mm:ss.xx> ...  最后一行最后一个词 <mm:ss.xx>
```

#### Implement

##### Data struct design
```c++
struct LyricLine{
    
};
```