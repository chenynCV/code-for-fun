#ifndef __LIB_RECORD_H__
#define __LIB_RECORD_H__

enum PATH_ID
{
    WORK_PATH = 1,//P'
    BACK_PATH = 2//P''
};

//�������¼��result������
extern void record_result(const PATH_ID path_id, const unsigned short edge);
//���result������Ϊ��ʼ״̬
extern void clear_result();

#endif
