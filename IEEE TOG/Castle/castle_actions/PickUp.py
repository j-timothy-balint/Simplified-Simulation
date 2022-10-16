
def applicability_condition(self,Agent,Object,Place):
	return SUCCESS

def preparatory_spec(self,Agent,Object,Place):
        if not contain(Place,Object):
                changeContents(Place,Object)
        if not contain(Place,Agent):
                changeContents(Place,agent)
	useObjects(self.id,(Agent,Object))
	return SUCCESS

def execution_steps(self,Agent,Object,Place):
	return {'PRIMITIVE':('Action',{'agents':Agent})}

def culmination_condition(self,Agent,Object,Place):
	if finishedAction(self.id):
		changeContentsMark(self.id,Agent,Object)
		return SUCCESS
	return INCOMPLETE

