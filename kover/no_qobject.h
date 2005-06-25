#ifndef _INTERFACE_H
#define _INTERFACE_H

class no_qobject {
public:
	virtual ~no_qobject() {};
	virtual void update_id(unsigned long) =0;
	virtual void set_status_text(const char *) =0;

};
#endif
