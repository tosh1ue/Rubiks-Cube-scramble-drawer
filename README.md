# 简介
本项目是一个超轻量的三阶魔方引擎，包含三阶魔方颜色信息存储结构、WCA标准打乱公式生成的方法（random-move）以及根据打乱公式更新魔方颜色信息的算法，配合前端可以制作魔方打乱图案生成器、魔方计时器等。占用资源小、无外部依赖，特别适合集成到单片机等资源紧张的平台。
# 特点
- **可维护性强**：算法层级清晰，分工明确
- **可移植性强**：仅依赖标准库，可嵌入到任何前端
- **资源占用小**：仅占用72字节静态存储空间，执行颜色更新时对数组原地操作，无额外存储开销
- **CPU缓存友好的内存布局**：采用连续的一维数组紧凑存储色块信息，最大化利用CPU缓存局部性原理，减少缓存未命中，相比多维数组分块存储方案有更稳定的访问性能
- **确定的时间复杂度**：单次旋转操作执行固定数量的内存单元交换，时间复杂度为O(1)，不随打乱步数增加而退化，适合高频调用场景
# 数据结构设计
本项目使用一个一维数组存储魔方的颜色信息，数据类型为uint8_t，代表魔方颜色对应的索引，实际的颜色值由前端进行定义。
### 面内序号
中心块颜色始终不变，无需存储，自LU色块开始顺时针开始标记，这是本项目实现算法抽象设计的关键。  
![](https://tosh1ue.github.io/pics/in-face-idx.png)  
这样在处理面内颜色信息交换时，只需根据旋转角度计算一个下标偏移。
```c
offset = (TURN_CYCLE - turn_angle) * 2;
```
例如旋转90度，对应旋转一次。偏移 = (4 - 1) * 2 = 6，即0变为6的颜色，6变为4的颜色，以此类推。
### 跨面序号
每个面的朝向经过特殊设计，这样可以使用同一套映射取得旋转时需要交换的色块序号。以WCA打乱的标准朝向（白顶绿前），魔方面的顺序为F-U-R-B-D-L。从F面开始，每个面相邻U面即为它的下一个面，中心块的标号为所处面的序号（也是中心块的颜色索引）。  
![](https://tosh1ue.github.io/pics/global-idx.jpg)  
特定色块在完整颜色信息数组中的序号为：面序号乘以每个面的色块数（不含中心块）加上色块在面内的序号。  
```c
sticker_idx_prefix = face_idx * STICKER_PER_FACE;
sticker_color = cube[sticker_idx_prefix + sticker_idx];
```
例如获取红色面的RU色块当前的颜色信息：
```c
cube_color_t color = cube[CUBE_FACE_R_IDX * STICKER_PER_FACE + CUBE_STICKER_RU_IDX];
```
# 算法设计
`move_applier`部分的算法采用**自顶向下**的设计，接收WCA标准打乱公式（有/无空格都兼容），逐步解析并执行原地颜色信息更新。
### 第一层：解析打乱字符串
将打乱公式字符串拆解成R、R2、R'等单步打乱。
```c
void cube_update_color(const char *scramble_alg);
```
### 第二层：翻译单步打乱
将单步打乱翻译成旋转面和旋转角度。
```c
static void cube_parse_step(const char face_char, const char suffix_char);
```
### 第三层：执行颜色更新
根据旋转面和旋转角度，确定需要交换的相邻面序号及需要交换的色块的序号，执行颜色更新。
```c
static void cube_turn(const cube_face_t turn_face, const cube_turn_t turn_angle);
```  

---

`scramble_generator`部分使用random-move，生成随机打乱步骤的打乱公式。虽然无法确保魔方的随机状态，但random-move结合一定的过滤规则生成的打乱公式已经基本可用，所以对于单片机等资源紧张的平台来说是一个不错的选择。  
当前的过滤规则如下：  
- 禁止连续两个旋转操作都在同一面，例如`R R'`
- 禁止连续三个旋转操作都在相对面，例如`R L R`

随机步骤生成过程用到了拒绝采样来保证随机数出现的概率一致，为了方便过滤无效打乱，分别随机生成旋转面和旋转角度，再组合成打乱字符串。如此往复，直到打乱公式长度满足要求。
# API
生成打乱公式：
```c
void cube_generate_scramble(char *scramble_alg, uint8_t len)：
```
重置魔方颜色：
```c
void cube_reset_color(void);
```
根据打乱公式更新魔方颜色：
```c
void cube_update_color(char *scramble_alg);
```
获取指向存储魔方颜色信息的数组的只读指针：
```c
const cube_color_t *cube_get_color(void);
```
# 示例代码
example.c提供了两个前端绘制方法  

将魔方颜色信息转换为数字字符串输出：  
```c
void print_cube_as_num(const cube_color_t *cube);
```
将魔方颜色信息转换为ANSI颜色代码输出：  
```c
void print_cube_with_color(const cube_color_t *cube);
```
由于部分终端输出不支持ANSI颜色代码，所以main方法中默认调用`print_cube_as_num`进行输出。  
# 使用
```bash
git clone https://github.com/tosh1ue/rubiks-cube-scramble-engine.git
cd rubiks-cube-scramble-engine
cmake -B build -S .
cmake --build build
./build/rubiks_cube_engine
```