
def applicability_condition(self,Agent,Subject,Place):
	return SUCCESS

def preparatory_spec(self,Agent,Subject,Place):
        if not contain(Place,Agent):
                changeContents(Place,Agent)
        if not contain(Place,Subject):
                changeContents(Place,Subject)
	useObjects(self.id,(Agent,Subject))
	return SUCCESS

def execution_steps(self,Agent,Subject,Place):
	return {'PRIMITIVE':('Action',{'agents':Agent})}

def culmination_condition(self,Agent,Subject,Place):
	if finishedAction(self.id):
		return SUCCESS
	return INCOMPLETE

