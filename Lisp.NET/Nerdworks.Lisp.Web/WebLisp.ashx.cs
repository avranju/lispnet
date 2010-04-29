using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.IO;
using Nerdworks.Lisp.Runtime;

namespace Nerdworks.Lisp.Web
{
    public class WebLisp : IHttpHandler
    {
        static LispRuntime runtime;

        static WebLisp()
        {
            //
            // boot lisp
            //
            LispRuntime.Boot();

            //
            // create and save a new lisp runtime object
            //
            runtime = new LispRuntime();
        }

        public void ProcessRequest(HttpContext context)
        {
            try
            {
                //
                // retrieve the lisp form to be evaluated from the post data
                //
                using (StreamReader reader = new StreamReader(context.Request.InputStream))
                {
                    EvalResult result = runtime.Eval(reader.ReadToEnd());
                    context.Response.ContentType = "application/json";
                    context.Response.Write(result.ToJson());
                }
            }
            catch (Exception ex)
            {
                context.Response.Write(new EvalResult
                {
                    Success = false,
                    ErrorOutput = ex.ToString()
                }.ToJson());
            }
        }

        public bool IsReusable
        {
            get
            {
                return true;
            }
        }
    }
}
