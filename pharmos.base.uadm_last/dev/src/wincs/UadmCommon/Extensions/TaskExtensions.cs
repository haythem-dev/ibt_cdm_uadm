using System;
using System.Threading.Tasks;
using UadmCommon.Classes;

namespace UadmCommon.Extensions
{
    public static class UadmTaskExtensions
    {
        /// <summary>
        /// WithTimeout
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="task"></param>
        /// <param name="timeoutMilliseconds"></param>
        /// <returns></returns>
        /// <exception cref="TimeoutException"></exception>
        public static async Task<T> WithTimeout<T>(this Task<T> task, int timeoutMilliseconds)
        {
            Task timeoutTask = Task.Delay(timeoutMilliseconds);
            Task completedTask = await Task.WhenAny(task, timeoutTask);

            if (completedTask == timeoutTask)
            {
                UadmLogger.LogError("Task timed out after " + timeoutMilliseconds + " milliseconds.");  

                throw new TimeoutException("The operation timed out.");
            }

            return await task; // Return the result of the original task
        }

        /// <summary>
        /// WithTimeoutAndRetry
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="operation"></param>
        /// <param name="timeoutMilliseconds"></param>
        /// <param name="maxRetries"></param>
        /// <returns></returns>
        /// <exception cref="InvalidOperationException"></exception>
        public static async Task<T> WithTimeoutAndRetry<T>(Func<Task<T>> operation, int timeoutMilliseconds, int maxRetries)
        {
            int tryCount = 0;

            // Retry loop
            while (tryCount < maxRetries)
            {
                try
                {
                    tryCount++;

                    Task timeoutTask = Task.Delay(  timeoutMilliseconds); // Create a timeout task
                    Task<T> operationTask = operation(); // Start the operation

                    Task completedTask = await Task.WhenAny(operationTask, timeoutTask); // Wait for either to complete

                    if (completedTask == timeoutTask)
                    {
                        string message = $"Attempt {tryCount} timed out after {timeoutMilliseconds} milliseconds.";

                        // Log the timeout event
                        UadmLogger.LogWarning(message);

                        if (tryCount >= maxRetries)
                            throw new TimeoutException(message);
                    }

                    return await operationTask; // Return the result if the operation succeeds
                }
                catch (Exception ex)
                {
                    if (tryCount >= maxRetries)
                    {
                        // Log the error and rethrow or handle it
                        UadmLogger.LogError($"Operation failed after {tryCount} attempts: {ex.Message}");
                        throw;
                    }
                }
            }

            throw new InvalidOperationException("This code should not be reached.");
        }
    }
}
