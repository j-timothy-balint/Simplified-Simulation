
def applicability_condition(self,Agent,Place):
	return SUCCESS

def preparatory_spec(self,Agent,Place):
        if not contain(Place,Agent):
                changeContents(Place,Agent)
	useObjects(self.id,(Agent))
	return SUCCESS

def execution_steps(self,Agent,Place):
	return {'PRIMITIVE':('Action',{'agents':Agent})}

def culmination_condition(self,Agent,Place):
	if finishedAction(self.id):
		return SUCCESS
	return INCOMPLETE

