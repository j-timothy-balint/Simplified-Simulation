
def applicability_condition(self,Agent,Agent1,Place):
	return SUCCESS

def preparatory_spec(self,Agent,Agent1,Place):
        if not contain(Place,Agent):
                changeContents(Place,Agent)
        if not contain(Place,Agent1):
                changeContents(Place,Agent1)
	useObjects(self.id,(Agent,Agent1))
	return SUCCESS

def execution_steps(self,Agent,Agent1,Place):
	return {'PRIMITIVE':('Action',{'agents':Agent})}

def culmination_condition(self,Agent,Agent1,Place):
	if finishedAction(self.id):
		return SUCCESS
	return INCOMPLETE

