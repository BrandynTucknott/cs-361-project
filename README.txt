Communication Contract:
1. Communication will primarily occur via Discord messages. 
2. When one of us posts a question, the other team member should respond within 24 hours. 
3. Feedback should be constructive and respectful.
4. We'll use a version control system (like Git) and regularly push our code, ensuring both team members can access the most recent version of code.
5. Other important documents such as this will be shared via the Canvas Discussion Group page.
6. Since most assignments are due Monday at midnight, it would be a good idea to have a plan developed for group assignments by the preceding Tuesday at midnight. This allows us to anticipate any task dependencies the other team member must complete.

Requesting data for your microservice:
* will be called via python subprocess method in sys library
* result = subprocess.run([sys.executable, "viewCalendarSummary.py", start_date, end_date], capture_output=True, text=True, check=True)
Receiving data for your microservice:
* returns a two numbers in JSON form, also through sys library subprocess method
* through the call above, the JSON string will be returned to the value 'result'