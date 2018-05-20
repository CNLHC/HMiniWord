#ifndef MINIWORDDOCUMENT_H
#define MINIWORDDOCUMENT_H

/*! \mainpage 概要设计
*
* \section 待办事项
* ### 继续实现题目要求的功能
* 1. 字符的查找和替换
* 2. 文件的相关操作(从文件读入内容，将缓冲区内容写入文件...)
* 3. 定位光标
* ### 目前存在的Bug
* 1. 在输入时按住鼠标会导致程序崩溃(MouseEvent和更新操作冲突，可引入互斥锁解决)
* 已解决√
* 2.
* 屏幕在纵向扩展时，不会随光标的移动而改变。(需要修改QScrollView的相关参数)
* 已解决√
* 3. 屏幕在纵向扩展时，光标定位会出现问题。(同上) 已解决√
* 4. 删除位于中间的空行时，会导致所有行被删除。 已解决√
*
*
*
*
*/

#endif // MINIWORDDOCUMENT_H
