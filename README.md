# 简介
本项目设计了三阶魔方颜色信息存储结构和根据打乱公式更新魔方颜色信息的算法，配合前端可以制作魔方打乱图案生成器、魔方动画渲染器等。
# 特点
- **可维护性强**：算法层级清晰，分工明确
- **可移植性强**：仅依赖标准库，可嵌入到任何前端
- **资源占用小**：仅占用72字节静态存储空间
- **CPU缓存友好的内存布局**：采用连续的一维数组紧凑存储色块信息，最大化利用CPU缓存局部性原理，减少缓存未命中，相比多维数组分块存储方案有更稳定的访问性能
- **确定的时间复杂度**：单次旋转操作执行固定数量的内存单元交换，时间复杂度为O(1)，不随打乱步数增加而退化，适合高频调用场景
# 数据结构设计
### 面内序号
中心块颜色始终不变，无需存储，自FLU色块开始顺时针开始标记，这是本项目实现算法抽象设计的关键。  
![](https://raw.githubusercontent.com/tosh1ue/pics/main/in-face-idx.png)

这样在处理面内颜色信息交换时，只需根据旋转角度计算一个下标偏移。
```c
offset = (TURN_CYCLE - turn_degree) * 2;
```
例如旋转90度，对应旋转一次。偏移 = (4 - 1) * 2 = 6，即0变为6的颜色，6变为4的颜色，以此类推。
### 跨面序号
每个面的朝向经过特殊设计，这样可以使用同一套映射取得旋转时需要交换的色块序号。中心块的标号为所处面的序号。  
![](https://raw.githubusercontent.com/tosh1ue/pics/main/global-idx.jpg)   

特定色块在完整颜色信息数组中的序号为：面序号乘以每个面的色块数（不含中心块）加上色块在面内的序号。  
```c
sticker_idx_prefix = face_idx * STICKER_PER_FACE;
sticker_color = cube[sticker_idx_prefix + sticker_idx];
```
例如获取红色面的FRU色块当前的颜色信息：
```c
cube_color_t color = cube[CUBE_FACE_R_IDX * STICKER_PER_FACE + CUBE_STICKER_FRU_IDX];
```
# 算法设计
算法采用**自顶向下**的设计。
### 第一层：解析打乱字符串
将打乱公式字符串拆解成R、R2、R'等单步打乱。
```c
void cube_update_color(char* scramble_alg);
```
### 第二层：翻译单步打乱
将单步打乱翻译成旋转面和旋转角度。
```c
void cube_parse_step(const char* scramble_step);
```
### 第三层：执行颜色更新
根据正视面和旋转角度，确定需要交换的相邻面序号及需要交换的色块的序号，执行颜色更新。
```c
void cube_turn(const cube_face_t turn_face, const cube_turn_t turn_degree);
```
# API
重置魔方颜色：
```c
void cube_reset_color(void);
```
根据打乱公式更新魔方颜色：
```c
void cube_update_color(char *scramble_alg);
```
获取指向存储魔方颜色信息的数组的指针：
```c
const cube_color_t* cube_get_color(void);
```
# 示例
example.c提供了两个前端绘制方法：
```c
void print_cube_as_num(const cube_color_t* cube);

void print_cube_with_color(const cube_color_t* cube);
```
由于部分终端输出不支持ANSI颜色代码，所以main方法中默认调用`print_cube_as_num`进行输出。
# 注意事项
色块的颜色信息使用单字节无符号整型uint8_t存储，部分编译器在使用%d整型占位符时可能会警告甚至报错，所以在使用时最好自己强转成int类型。
```c
printf("%d", (int)cube[idx]);
```
