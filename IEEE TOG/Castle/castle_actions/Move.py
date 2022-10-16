
def applicability_condition(self,Agent,From,To):
	return SUCCESS

def preparatory_spec(self,Agent,From,To):
        if not contain(From,Agent):
                changeContents(From,Agent)
        useObjects(self.id,(Agent))
	return SUCCESS

def execution_steps(self,Agent,From,To):
	return {'PRIMITIVE':('Action',{'agents':Agent})}

def culmination_condition(self,Agent,From,To):
	if finishedAction(self.id):
		changeContentsMark(self.id,To,Agent)
		return SUCCESS
	return INCOMPLETE

