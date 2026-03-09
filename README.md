# 简介
本项目设计了一个三阶魔方颜色信息存储结构和根据打乱公式更新魔方颜色信息的方法，配合前端可以制作打乱图案生成器。
# 数据结构设计
使用一维数组进行存储，保证内存连续性。
### 面内序号
中心块颜色始终不变，无需存储，自FLU色块开始顺时针开始标记，这是本项目实现高效算法设计的关键。
![cube_ManimCE_v0.20.1 - 2副本.png](https://picui.ogmua.cn/s1/2026/03/09/69aee9938297d.webp)

这样在处理面内颜色信息交换时，只需根据旋转角度计算一个下标偏移。

`offset = (TURN_CYCLE - turn_degree) * 2;`

例如旋转90度，对应旋转一次。偏移 = (4 - 1) * 2 = 6，即0变为6的颜色，6变为4的颜色，以此类推。
### 跨面序号
特定色块在完整颜色信息数组中的序号为：面序号乘以每个面的色块数（不含中心块）加上色块在面内的序号。

    stickerIdxPrefix = face_idx * CUBE_FACE_STICKER_NUM;
    cube_color_t sticker_color = cube[stickerIdxPrefix + sticker_idx];
例如获取第二个面的第三个色块当前的颜色信息：
`cube_color_t color = cube[1 * 8 + 2];`

# 算法设计
算法采用**自顶向下**的设计。
### 第一层：解析打乱字符串
`void cube_update_color(char* scramble_alg);`

将打乱公式字符串拆解成R、R2、R'等单步打乱。
### 第二层：翻译单步打乱
`void cube_parse_step(const char* scramble_step);`

将单步打乱翻译成正视面和旋转角度。
### 第三层：执行颜色更新
`void cube_turn(const cube_face_t facing_idx, const cube_turn_t turn_degree);`

根据正视面和旋转角度，确定需要交换的相邻面序号及需要交换的色块的序号，执行颜色更新。
# API
重置魔方颜色：
`void cube_reset_color(void);`

根据打乱公式更新魔方颜色：
`void void cube_update_color(char *scramble_alg);`

获取指向存储魔方颜色信息的数组的指针：
`const cube_t* cube_get_color(void);`
# 示例
example.c提供了两个前端绘制方法：

`void (const cube_t* cube);`

`void print_cube_with_color(const cube_t* cube);`

由于部分终端输出不支持ANSI颜色代码，所以main方法中默认调用`print_cube_as_num`进行输出。
