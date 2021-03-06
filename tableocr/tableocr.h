#pragma once
#ifndef TABLEOCR_API
	#define TABLEOCR_API extern "C" __declspec(dllimport)
#endif

// 选项设置
typedef struct _TOOPTION
{
	BOOL isMerged;	// 是否包含合并的单元格，如果不能确定，设为TRUE
	int minSize;	// 最小行高
	int threshold;	// Hough检测的阈值,建议设为5-10
	int delta;		// 考虑线宽、倾斜、不清晰等因素时容错的像素范围，建议设为3
}TOOPTION;

/*
加载图片并初始化。
参数说明：
pszImagePath	[in, string] 图片路径
pOption			[in] 选项设置
pSize			[out] 接收图片的大小
返回值：
OCR处理句柄，该句柄使用完毕后通过TOFree函数释放。如果失败，返回NULL。
*/
TABLEOCR_API void* WINAPI TOLoadImage(const char* pszImagePath, const TOOPTION* pOption, LPSIZE pSize);

/*
释放OCR处理句柄。
参数说明：
hTO				[in, out] TOLoadImage返回的OCR处理句柄
*/
TABLEOCR_API void WINAPI TOFree(void* hTO);

/*
获取图片中表格的数量。
参数说明：
hTO				[in, out] TOLoadImage返回的OCR处理句柄
返回值：
表格的数量。
*/
TABLEOCR_API int WINAPI TOGetTableCount(void* hTO);

/*
根据索引获取表格。
参数说明：
hTO				[in, out] TOLoadImage返回的OCR处理句柄
index			[in] 表格在图片中的索引，从0开始
pRect			[out] 表格在图片中的位置和大小
返回值：
表格的句柄。该句柄不需要释放。
*/
TABLEOCR_API void* WINAPI TOGetTable(void* hTO, int index, LPRECT pRect);

/*
获取表格的行数。
参数说明：
hTable			[in, out] TOGetTable返回的表格句柄
返回值：
表格的行数。
*/
TABLEOCR_API int WINAPI TOGetRowCount(void* hTable);

/*
获取表格的列数。
参数说明：
hTable			[in, out] TOGetTable返回的表格句柄
返回值：
表格的列数。
*/
TABLEOCR_API int WINAPI TOGetColumnCount(void* hTable);

/*
获取表格指定行的行高。
参数说明：
hTable			[in, out] TOGetTable返回的表格句柄
iRow			[in] 行的索引，从0开始
返回值：
该行的行高。
*/
TABLEOCR_API int WINAPI TOGetRowHeight(void* hTable, int iRow);

/*
获取表格指定列的列宽。
参数说明：
hTable			[in, out] TOGetTable返回的表格句柄
iCol			[in] 列的索引，从0开始
返回值：
该列的列宽。
*/
TABLEOCR_API int WINAPI TOGetColumnWidth(void* hTable, int iCol);

/*
获取单元格的位置和大小。
参数说明：
hTable			[in, out] TOGetTable返回的表格句柄
iRow			[in] 行的索引，从0开始，且不受其他行单元格合并的影响
iCol			[in] 列的索引，从0开始，且不受其他列单元格合并的影响
pRect			[out] 接收单元格的位置和大小，如果单元格被合并，返回的是合并后的位置和大小
返回值：
如果单元格未被合并，返回TRUE。如果多个单元格合并，则仅最左上的单元格返回TRUE，其他单元格返回FALSE。
*/
TABLEOCR_API BOOL WINAPI TOGetCell(void* hTable, int iRow, int iCol, LPRECT pRect);

/*
调用OpenCV加粗显示识别的表格，通常用于调试。
参数说明：
hTable			[in, out] TOGetTable返回的表格句柄
winname			[in, string] 窗口名字，如果要显示多个窗口，应使用不同名字
ratio			[in] 图像缩小比例，1表示不变
*/
TABLEOCR_API void WINAPI TODrawTable(void* hTable, const char* winname, int ratio);

/*
识别指定区域的文字。
参数说明：
hTO				[in, out] TOLoadImage返回的OCR处理句柄
pRect			[in] 区域的位置和大小
返回值：
识别的字符串。该字符串缓冲区在下次调用TOOcr前有效。
*/
TABLEOCR_API const char* WINAPI TOOcr(void* hTO, const RECT* pRect);